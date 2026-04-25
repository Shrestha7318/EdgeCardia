/*
 * main.c
 */

#include "stm32f4xx.h"
#include "ecg_hardware.h"
#include <math.h>
#include <stdio.h>
#include "filters.h"
#include "normalization.h"
#include "random_forest.h"
#include "thresholds.h"
#include "i2c.h"
#include "display_ui.h"

float apply_biquad_filter(float input, const float b[3], const float a[3], float state[2]);

#define SAMPLING_RATE        250
#define BUFFER_SIZE          250
#define NUM_FEATURES         8
#define SMOOTH_WINDOW        5
#define OLED_UPDATE_SAMPLES  25
#define REPORT_SAMPLES       500

float ecg_buffer[BUFFER_SIZE];
uint16_t buffer_index = 0;
float rr_intervals[10];
uint8_t rr_count = 0;
uint32_t last_r_peak_sample = 0;

float r_peak_threshold = 0.3f;
uint8_t in_peak = 0;
float peak_max_val = 0.0f;
uint32_t peak_max_sample = 0;
uint32_t min_rr_samples = 75;
float last_qrs_width_ms = 80.0f;

float hp_state[2] = {0, 0};
float lp_state[2] = {0, 0};

uint8_t rhythm_history[SMOOTH_WINDOW] = {0};
uint8_t copd_history[SMOOTH_WINDOW] = {0};
uint8_t age_history[SMOOTH_WINDOW] = {0};
uint8_t smooth_idx = 0;

typedef struct {
    float bpm;
    uint8_t rhythm;
    float stress_level;
    uint8_t copd_risk;
    uint8_t heart_age;
} Results_t;

Results_t results = {0};

float ema_bpm = 0.0f;
float ema_sdnn = 0.0f;

#define EMA_ALPHA 0.3f

static uint8_t leads_connected_last = 0;
static DisplayMode_t display_mode = DISPLAY_MODE_STATUS;

void SystemClock_Config(void);
void UART_Init(void);
void UART_SendString(const char* str);
void UART_SendInt(int val);
void UART_SendFloat(float val);
void delay_ms(uint32_t ms);
void extract_features(float* features);
void calculate_health_metrics(void);

int main(void) {
    SystemClock_Config();
    UART_Init();
    ECG_Hardware_Init();

    i2c_init();
    Display_UI_Init();
    Display_Button_Init();
    Display_UI_ShowBoot();

    UART_SendString("\r\nEdgeCardia HW Ready\r\n");

    uint32_t sample_counter = 0;
    uint32_t report_counter = 0;
    uint32_t oled_counter = 0;

    SysTick->LOAD = (84000000 / 250) - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    while (1) {
        if (Display_Button_Pressed()) {
            if (display_mode == DISPLAY_MODE_STATUS) {
                display_mode = DISPLAY_MODE_WAVE;
                UART_SendString("OLED: WAVEFORM MODE\r\n");
            } else {
                display_mode = DISPLAY_MODE_STATUS;
                UART_SendString("OLED: STATUS MODE\r\n");
            }

            oled_counter = OLED_UPDATE_SAMPLES;
        }

        uint8_t leads_connected = ECG_Hardware_LeadsConnected();

        if (!leads_connected) {
            if (leads_connected_last) {
                UART_SendString("LEADS OFF\r\n");
                leads_connected_last = 0;
            }

            results.bpm = 0.0f;
            results.rhythm = 0;
            results.stress_level = 0.0f;
            results.copd_risk = 0;
            results.heart_age = 0;

            oled_counter++;
            report_counter++;

            if (oled_counter >= OLED_UPDATE_SAMPLES) {
                oled_counter = 0;
                Display_UI_ShowNoLeads();
            }

            if (report_counter >= REPORT_SAMPLES) {
                report_counter = 0;
                UART_SendString("BPM:0 R:0 S:0 C:0 A:0\r\n");
            }

            while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
            continue;
        }

        if (!leads_connected_last) {
            UART_SendString("LEADS ON\r\n");
            leads_connected_last = 1;
        }

        uint16_t adc_value = ECG_Hardware_GetSample();

        float voltage = (adc_value / 4095.0f) * 3.3f;
        float normalized = (voltage - 1.65f) / 1.65f;

        float filtered = apply_biquad_filter(normalized, b_hp, a_hp, hp_state);
        filtered = apply_biquad_filter(filtered, b_lp, a_lp, lp_state);

        ecg_buffer[buffer_index] = filtered;
        buffer_index = (buffer_index + 1) % BUFFER_SIZE;

        sample_counter++;
        report_counter++;
        oled_counter++;

        if (sample_counter % 4 == 0) {
            UART_SendString("E:");
            UART_SendFloat(filtered);
            UART_SendString("\r\n");
        }

        if (!in_peak && filtered > r_peak_threshold) {
            in_peak = 1;
            peak_max_val = filtered;
            peak_max_sample = sample_counter;
        }

        if (in_peak && filtered > peak_max_val) {
            peak_max_val = filtered;
            peak_max_sample = sample_counter;
        }

        if (in_peak && filtered < (peak_max_val * 0.3f)) {
            in_peak = 0;

            if (last_r_peak_sample == 0 ||
                (peak_max_sample - last_r_peak_sample) > min_rr_samples) {

                float rr_ms = ((peak_max_sample - last_r_peak_sample) * 1000.0f) / SAMPLING_RATE;

                if (last_r_peak_sample > 0 && rr_ms > 300.0f && rr_ms < 2000.0f) {
                    if (rr_count < 10) {
                        rr_intervals[rr_count] = rr_ms;
                        rr_count++;
                    } else {
                        for (int i = 0; i < 9; i++) {
                            rr_intervals[i] = rr_intervals[i + 1];
                        }
                        rr_intervals[9] = rr_ms;
                    }
                }

                last_r_peak_sample = peak_max_sample;

                uint16_t r_buf_idx = peak_max_sample % BUFFER_SIZE;
                float half_peak = peak_max_val * 0.5f;
                uint8_t qrs_samples = 1;

                for (int i = 1; i < 20; i++) {
                    uint16_t idx = (r_buf_idx - i + BUFFER_SIZE) % BUFFER_SIZE;
                    if (ecg_buffer[idx] > half_peak) {
                        qrs_samples++;
                    } else {
                        break;
                    }
                }

                for (int i = 1; i < 20; i++) {
                    uint16_t idx = (r_buf_idx + i) % BUFFER_SIZE;
                    if (ecg_buffer[idx] > half_peak) {
                        qrs_samples++;
                    } else {
                        break;
                    }
                }

                last_qrs_width_ms = qrs_samples * (1000.0f / SAMPLING_RATE);

                r_peak_threshold = peak_max_val * 0.6f;
                if (r_peak_threshold < 0.15f) r_peak_threshold = 0.15f;
                if (r_peak_threshold > 1.0f) r_peak_threshold = 1.0f;
            }

            peak_max_val = 0.0f;
        }

        if (!in_peak) {
            r_peak_threshold *= 0.999f;
            if (r_peak_threshold < 0.15f) r_peak_threshold = 0.15f;
        }

        if (sample_counter % REPORT_SAMPLES == 0 && rr_count >= 5) {
            calculate_health_metrics();
        }

        if (report_counter >= REPORT_SAMPLES) {
            report_counter = 0;

            if (results.bpm > 0) {
                UART_SendString("BPM:");
                UART_SendInt((int)results.bpm);
                UART_SendString(" R:");
                UART_SendInt(results.rhythm);
                UART_SendString(" S:");
                UART_SendInt((int)results.stress_level);
                UART_SendString(" C:");
                UART_SendInt(results.copd_risk);
                UART_SendString(" A:");
                UART_SendInt(results.heart_age);
                UART_SendString("\r\n");
            } else {
                UART_SendString("BPM:0 R:0 S:0 C:0 A:0\r\n");
            }
        }

        if (oled_counter >= OLED_UPDATE_SAMPLES) {
            oled_counter = 0;

            if (display_mode == DISPLAY_MODE_STATUS) {
                Display_UI_ShowStatus(
                    (int)results.bpm,
                    results.rhythm,
                    (int)results.stress_level,
                    results.copd_risk,
                    results.heart_age,
                    1,
                    0
                );
            } else {
                Display_UI_ShowWave(
                    ecg_buffer,
                    BUFFER_SIZE,
                    buffer_index,
                    (int)results.bpm,
                    1
                );
            }
        }

        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
    }
}

void extract_features(float* features) {
    if (rr_count < 5) return;

    features[0] = rr_intervals[rr_count - 1];

    float sorted[10];

    for (int i = 0; i < rr_count; i++) {
        sorted[i] = rr_intervals[i];
    }

    for (int i = 0; i < rr_count - 1; i++) {
        for (int j = 0; j < rr_count - 1 - i; j++) {
            if (sorted[j] > sorted[j + 1]) {
                float tmp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = tmp;
            }
        }
    }

    int trim_start = (rr_count > 4) ? 1 : 0;
    int trim_end = (rr_count > 4) ? rr_count - 1 : rr_count;
    int trim_count = trim_end - trim_start;

    float mean = 0.0f;

    for (int i = trim_start; i < trim_end; i++) {
        mean += sorted[i];
    }

    mean /= trim_count;

    float variance = 0.0f;

    for (int i = trim_start; i < trim_end; i++) {
        float diff = sorted[i] - mean;
        variance += diff * diff;
    }

    features[1] = sqrtf(variance / trim_count);

    float sum_sq_diff = 0.0f;

    for (int i = 0; i < rr_count - 1; i++) {
        float diff = rr_intervals[i + 1] - rr_intervals[i];
        sum_sq_diff += diff * diff;
    }

    features[2] = sqrtf(sum_sq_diff / (rr_count - 1));

    uint16_t r_idx = last_r_peak_sample % BUFFER_SIZE;

    features[3] = last_qrs_width_ms;

    uint16_t p_idx = (r_idx - 37 + BUFFER_SIZE) % BUFFER_SIZE;
    features[4] = ecg_buffer[p_idx];

    uint16_t t_idx = (r_idx + 62) % BUFFER_SIZE;
    features[5] = ecg_buffer[t_idx];

    features[6] = ecg_buffer[r_idx];

    float energy = 0.0f;

    for (int i = 0; i < BUFFER_SIZE; i++) {
        energy += ecg_buffer[i] * ecg_buffer[i];
    }

    features[7] = energy / BUFFER_SIZE;
}

void calculate_health_metrics(void) {
    float features[NUM_FEATURES];

    extract_features(features);

    if (rr_count > 0) {
        float instantaneous_bpm = 60000.0f / rr_intervals[rr_count - 1];

        if (ema_bpm < 1.0f) {
            ema_bpm = instantaneous_bpm;
        } else {
            ema_bpm = EMA_ALPHA * instantaneous_bpm + (1.0f - EMA_ALPHA) * ema_bpm;
        }

        results.bpm = ema_bpm;
    }

    float raw_sdnn = features[1];
    float raw_qrs_width = features[3];
    float raw_p_amplitude = features[4];

    if (ema_sdnn < 0.01f) {
        ema_sdnn = raw_sdnn;
    } else {
        ema_sdnn = EMA_ALPHA * raw_sdnn + (1.0f - EMA_ALPHA) * ema_sdnn;
    }

    results.stress_level = ema_sdnn;

    normalize_features(features);

    uint8_t raw_rhythm = predict_random_forest(features);
    rhythm_history[smooth_idx] = raw_rhythm;

    uint8_t arrhythmia_votes = 0;

    for (int i = 0; i < SMOOTH_WINDOW; i++) {
        arrhythmia_votes += rhythm_history[i];
    }

    results.rhythm = (arrhythmia_votes >= (SMOOTH_WINDOW / 2 + 1)) ? 1 : 0;

    float p_zscore = (raw_p_amplitude - feature_means[4]) / feature_stds[4];
    uint8_t raw_copd = (p_zscore > 1.0f) ? 1 : 0;

    copd_history[smooth_idx] = raw_copd;

    uint8_t copd_votes = 0;

    for (int i = 0; i < SMOOTH_WINDOW; i++) {
        copd_votes += copd_history[i];
    }

    results.copd_risk = (copd_votes >= (SMOOTH_WINDOW / 2 + 1)) ? 1 : 0;

    float qrs_zscore = (raw_qrs_width - feature_means[3]) / feature_stds[3];

    uint8_t raw_age;

    if (qrs_zscore > 2.0f) {
        raw_age = 2;
    } else if (qrs_zscore > 1.0f) {
        raw_age = 1;
    } else {
        raw_age = 0;
    }

    age_history[smooth_idx] = raw_age;

    uint8_t age_counts[3] = {0, 0, 0};

    for (int i = 0; i < SMOOTH_WINDOW; i++) {
        if (age_history[i] < 3) {
            age_counts[age_history[i]]++;
        }
    }

    if (age_counts[2] >= (SMOOTH_WINDOW / 2 + 1)) {
        results.heart_age = 2;
    } else if (age_counts[1] >= (SMOOTH_WINDOW / 2 + 1)) {
        results.heart_age = 1;
    } else {
        results.heart_age = 0;
    }

    smooth_idx = (smooth_idx + 1) % SMOOTH_WINDOW;
}

void SystemClock_Config(void) {
    RCC->CR |= RCC_CR_HSION;

    while (!(RCC->CR & RCC_CR_HSIRDY));

    FLASH->ACR = FLASH_ACR_LATENCY_2WS |
                 FLASH_ACR_ICEN |
                 FLASH_ACR_DCEN |
                 FLASH_ACR_PRFTEN;

    RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC_HSI |
                   (168 << RCC_PLLCFGR_PLLN_Pos) |
                   (16 << RCC_PLLCFGR_PLLM_Pos) |
                   (0 << RCC_PLLCFGR_PLLP_Pos) |
                   (7 << RCC_PLLCFGR_PLLQ_Pos);

    RCC->CR |= RCC_CR_PLLON;

    while (!(RCC->CR & RCC_CR_PLLRDY));

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

void UART_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    GPIOA->MODER |= (2 << GPIO_MODER_MODER2_Pos) |
                    (2 << GPIO_MODER_MODER3_Pos);

    GPIOA->AFR[0] &= ~((0xF << GPIO_AFRL_AFSEL2_Pos) |
                       (0xF << GPIO_AFRL_AFSEL3_Pos));

    GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos) |
                     (7 << GPIO_AFRL_AFSEL3_Pos);

    USART2->BRR = 0x016D;

    USART2->CR1 = USART_CR1_TE |
                  USART_CR1_RE |
                  USART_CR1_UE;
}

void UART_SendString(const char* str) {
    while (*str) {
        while (!(USART2->SR & USART_SR_TXE));
        USART2->DR = *str++;
    }
}

void UART_SendInt(int val) {
    char buf[12];
    int i = 0;

    if (val < 0) {
        UART_SendString("-");
        val = -val;
    }

    if (val == 0) {
        UART_SendString("0");
        return;
    }

    while (val > 0) {
        buf[i++] = '0' + (val % 10);
        val /= 10;
    }

    while (i > 0) {
        char c[2] = {buf[--i], 0};
        UART_SendString(c);
    }
}

void UART_SendFloat(float val) {
    if (val < 0) {
        UART_SendString("-");
        val = -val;
    }

    int integer_part = (int)val;
    int decimal_part = (int)((val - integer_part) * 1000);

    UART_SendInt(integer_part);
    UART_SendString(".");

    if (decimal_part < 100) UART_SendString("0");
    if (decimal_part < 10) UART_SendString("0");

    UART_SendInt(decimal_part);
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 10500; i++) {
        __NOP();
    }
}
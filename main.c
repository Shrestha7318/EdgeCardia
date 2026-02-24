/*
 * main.c
 */

#include "stm32f4xx.h"  
#include "ecg_hardware.h"  
#include <math.h>
#include "filters.h"
#include "normalization.h"
#include "random_forest.h"
#include "thresholds.h"

float apply_biquad_filter(float input, const float b[3], const float a[3], float state[2]);

#define SAMPLING_RATE 250
#define BUFFER_SIZE 250
#define NUM_FEATURES 8

float ecg_buffer[BUFFER_SIZE];
uint16_t buffer_index = 0;
float rr_intervals[10];
uint8_t rr_count = 0;
uint32_t last_r_peak_sample = 0;  

// R-peak detection state
float r_peak_threshold = 0.3f;   // adaptive threshold
uint8_t in_peak = 0;             // 1 = signal is currently above threshold
float peak_max_val = 0.0f;       // max value during current peak
uint32_t peak_max_sample = 0;    // sample_counter at max
uint32_t min_rr_samples = 75;   
float last_qrs_width_ms = 80.0f; // measured QRS width in ms (default 80ms = healthy)

// Filter states
float hp_state[2] = {0, 0};
float lp_state[2] = {0, 0};

// Smoothing windows for stable classification output
#define SMOOTH_WINDOW 5
uint8_t rhythm_history[SMOOTH_WINDOW] = {0};
uint8_t copd_history[SMOOTH_WINDOW] = {0};
uint8_t age_history[SMOOTH_WINDOW] = {0};
uint8_t smooth_idx = 0;

// Results
typedef struct {
    float bpm;
    uint8_t rhythm;
    float stress_level;
    uint8_t copd_risk;
    uint8_t heart_age;
} Results_t;

Results_t results = {0};

// Exponential moving average state for smooth output
float ema_bpm = 0.0f;
float ema_sdnn = 0.0f;
#define EMA_ALPHA 0.3f  // Smoothing factor: 0.3 

// Function prototypes
void SystemClock_Config(void);
void UART_Init(void);
void UART_SendString(const char* str);
void UART_SendInt(int val);
void UART_SendFloat(float val);
void delay_ms(uint32_t ms);
void extract_features(float* features);
void calculate_health_metrics(void);


int main(void) {
    // Initialize system
    SystemClock_Config();
    UART_Init();
    ECG_Hardware_Init();  // Initialize real ADC hardware
    
    UART_SendString("\r\nEdgeCardia HW Ready\r\n");
    
    uint32_t sample_counter = 0;
    uint32_t report_counter = 0;
    uint8_t leads_connected_last = 0;
    
    while(1) {
        // Lead-off detection temporarily disabled for debugging
        // Uncomment when electrodes are stable
        /*
        uint8_t leads_connected = ECG_Hardware_LeadsConnected();
        
        if (!leads_connected) {
            if (leads_connected_last) {
                UART_SendString("LEADS OFF\r\n");
                leads_connected_last = 0;
            }
            delay_ms(500);
            continue;
        }
        
        if (!leads_connected_last) {
            UART_SendString("LEADS ON\r\n");
            leads_connected_last = 1;
        }
        */
        
        // Read REAL ECG sample from AD8232
        uint16_t adc_value = ECG_Hardware_GetSample();
        
        // Convert to voltage (-1 to 1)
        float voltage = (adc_value / 4095.0f) * 3.3f;
        float normalized = (voltage - 1.65f) / 1.65f;
        
        // Apply filters
        float filtered = apply_biquad_filter(normalized, b_hp, a_hp, hp_state);
        filtered = apply_biquad_filter(filtered, b_lp, a_lp, lp_state);
        
        // Store in buffer
        ecg_buffer[buffer_index] = filtered;
        buffer_index = (buffer_index + 1) % BUFFER_SIZE;
        
        sample_counter++;
        
        // Stream ECG samples to dashboard (every 4th sample = ~62.5 Hz)
        if (sample_counter % 4 == 0) {
            UART_SendString("E:");
            UART_SendFloat(filtered);
            UART_SendString("\r\n");
        }
        
        // Detect when signal crosses above threshold (entering a peak)
        if (!in_peak && filtered > r_peak_threshold) {
            in_peak = 1;
            peak_max_val = filtered;
            peak_max_sample = sample_counter;
        }
        
        // Track the maximum during the peak
        if (in_peak && filtered > peak_max_val) {
            peak_max_val = filtered;
            peak_max_sample = sample_counter;
        }
        
        // Detect when signal falls back below threshold (peak ended)
        if (in_peak && filtered < r_peak_threshold * 0.5f) {
            in_peak = 0;
            
            // Ensure minimum distance from last R-peak (refractory period)
            if (last_r_peak_sample == 0 || 
                (peak_max_sample - last_r_peak_sample) > min_rr_samples) {
                
                // Calculate RR interval in ms
                float rr_ms = ((peak_max_sample - last_r_peak_sample) * 1000.0f) / SAMPLING_RATE;
                
                // Validate RR interval (30-200 BPM range)
                if (last_r_peak_sample > 0 && rr_ms > 300.0f && rr_ms < 2000.0f) {
                    if (rr_count < 10) {
                        rr_intervals[rr_count] = rr_ms;
                        rr_count++;
                    } else {
                        for (int i = 0; i < 9; i++) {
                            rr_intervals[i] = rr_intervals[i+1];
                        }
                        rr_intervals[9] = rr_ms;
                    }
                }
                
                last_r_peak_sample = peak_max_sample;
                
                // This estimates how wide the QRS complex is
                uint16_t r_buf_idx = peak_max_sample % BUFFER_SIZE;
                float half_peak = peak_max_val * 0.5f;
                uint8_t qrs_samples = 1; // the peak itself
                
                // Count samples to the left of peak above half-amplitude
                for (int i = 1; i < 20; i++) {
                    uint16_t idx = (r_buf_idx - i + BUFFER_SIZE) % BUFFER_SIZE;
                    if (ecg_buffer[idx] > half_peak) {
                        qrs_samples++;
                    } else {
                        break;
                    }
                }
                // Count samples to the right of peak above half-amplitude
                for (int i = 1; i < 20; i++) {
                    uint16_t idx = (r_buf_idx + i) % BUFFER_SIZE;
                    if (ecg_buffer[idx] > half_peak) {
                        qrs_samples++;
                    } else {
                        break;
                    }
                }
                
                // Convert samples to ms: samples * (1000 / SAMPLING_RATE)
                last_qrs_width_ms = qrs_samples * (1000.0f / SAMPLING_RATE);
                
                // Adapt threshold: 60% of the peak height
                r_peak_threshold = peak_max_val * 0.6f;
                if (r_peak_threshold < 0.1f) r_peak_threshold = 0.1f;
                if (r_peak_threshold > 1.0f) r_peak_threshold = 1.0f;
            }
            
            peak_max_val = 0.0f;
        }
        
        // Calculate health metrics periodically (every 2 seconds)
        // Only when we have enough RR intervals
        if (sample_counter % 500 == 0 && rr_count >= 5) {
            calculate_health_metrics();
        }
        
        // Print results every 2 seconds
        report_counter++;
        if (report_counter >= 500) {
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
        
        // Maintain 250Hz sampling rate
        delay_ms(4);
    }
}

// [Rest of the functions stay the same - detect_r_peak, extract_features, etc.]

// detect_r_peak is no longer needed - R-peak detection is now
// done inline in the main loop using threshold crossing

void extract_features(float* features) {
    if (rr_count < 5) return;
    
    features[0] = rr_intervals[rr_count - 1];
    
    // SDNN: use trimmed std dev — sort a copy, drop highest and lowest,
    // compute std dev of remaining. This prevents outlier RR intervals
    // (from noise) from inflating SDNN while keeping BPM accurate.
    float sorted[10];
    for (int i = 0; i < rr_count; i++) sorted[i] = rr_intervals[i];
    // Simple bubble sort (max 10 elements)
    for (int i = 0; i < rr_count - 1; i++) {
        for (int j = 0; j < rr_count - 1 - i; j++) {
            if (sorted[j] > sorted[j+1]) {
                float tmp = sorted[j];
                sorted[j] = sorted[j+1];
                sorted[j+1] = tmp;
            }
        }
    }
    
    // Trim: skip first and last (if we have enough data)
    int trim_start = (rr_count > 4) ? 1 : 0;
    int trim_end = (rr_count > 4) ? rr_count - 1 : rr_count;
    int trim_count = trim_end - trim_start;
    
    float mean = 0;
    for (int i = trim_start; i < trim_end; i++) mean += sorted[i];
    mean /= trim_count;
    
    float variance = 0;
    for (int i = trim_start; i < trim_end; i++) {
        float diff = sorted[i] - mean;
        variance += diff * diff;
    }
    features[1] = sqrtf(variance / trim_count);
    
    float sum_sq_diff = 0;
    for (int i = 0; i < rr_count - 1; i++) {
        float diff = rr_intervals[i+1] - rr_intervals[i];
        sum_sq_diff += diff * diff;
    }
    features[2] = sqrtf(sum_sq_diff / (rr_count - 1));
    
    uint16_t r_idx = last_r_peak_sample % BUFFER_SIZE;
    features[3] = last_qrs_width_ms;  // Actual measured QRS width
    
    uint16_t p_idx = (r_idx - 37 + BUFFER_SIZE) % BUFFER_SIZE;
    features[4] = ecg_buffer[p_idx];
    
    uint16_t t_idx = (r_idx + 62) % BUFFER_SIZE;
    features[5] = ecg_buffer[t_idx];
    
    features[6] = ecg_buffer[r_idx];
    
    float energy = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        energy += ecg_buffer[i] * ecg_buffer[i];
    }
    features[7] = energy / BUFFER_SIZE;
}

void calculate_health_metrics(void) {
    float features[NUM_FEATURES];
    
    extract_features(features);
    
    if (rr_count > 0) {
        // Use the most recent RR interval for BPM
        float instantaneous_bpm = 60000.0f / rr_intervals[rr_count - 1];
        
        // Apply EMA smoothing so BPM changes gradually
        if (ema_bpm < 1.0f) {
            ema_bpm = instantaneous_bpm;
        } else {
            ema_bpm = EMA_ALPHA * instantaneous_bpm + (1.0f - EMA_ALPHA) * ema_bpm;
        }
        results.bpm = ema_bpm;
    }
    
    // Save raw feature values BEFORE normalization for threshold-based checks
    float raw_sdnn = features[1];       // SDNN in ms
    float raw_qrs_width = features[3];  // QRS width in ms
    float raw_p_amplitude = features[4]; // P-wave amplitude (raw voltage)
    
    results.stress_level = raw_sdnn;
    
    // Apply EMA smoothing to SDNN so it doesn't jump wildly
    if (ema_sdnn < 0.01f) {
        ema_sdnn = raw_sdnn;  // Initialize on first reading
    } else {
        ema_sdnn = EMA_ALPHA * raw_sdnn + (1.0f - EMA_ALPHA) * ema_sdnn;
    }
    results.stress_level = ema_sdnn;
    
    // Normalize for ML model
    normalize_features(features);
    
    uint8_t raw_rhythm = predict_random_forest(features);
    rhythm_history[smooth_idx] = raw_rhythm;
    
    uint8_t arrhythmia_votes = 0;
    for (int i = 0; i < SMOOTH_WINDOW; i++) {
        arrhythmia_votes += rhythm_history[i];
    }
    // Only flag arrhythmia if majority of recent readings agree
    results.rhythm = (arrhythmia_votes >= (SMOOTH_WINDOW / 2 + 1)) ? 1 : 0;
    
    // The ML model's normalization tells us the training distribution:
    //   P-amplitude mean = -0.1102, std = 0.2778
    // If the z-score is > 1.0 (P-wave more than 1 std above normal),
    // that correlates with P-pulmonale, a COPD indicator.
    float p_zscore = (raw_p_amplitude - feature_means[4]) / feature_stds[4];
    uint8_t raw_copd = (p_zscore > 1.0f) ? 1 : 0;
    copd_history[smooth_idx] = raw_copd;
    
    uint8_t copd_votes = 0;
    for (int i = 0; i < SMOOTH_WINDOW; i++) {
        copd_votes += copd_history[i];
    }
    results.copd_risk = (copd_votes >= (SMOOTH_WINDOW / 2 + 1)) ? 1 : 0;
    
    // QRS width mean = 110.2ms, std = 34.1ms from training data.
    // Normal young heart: QRS near or below mean (z <= 0)
    // Aging conduction: QRS widens above mean
    //   z > 1.0 std above mean (~144ms) = Middle age conduction
    //   z > 2.0 std above mean (~178ms) = Elderly conduction pattern
    float qrs_zscore = (raw_qrs_width - feature_means[3]) / feature_stds[3];
    uint8_t raw_age;
    if (qrs_zscore > 2.0f) {
        raw_age = 2;  // Elderly
    } else if (qrs_zscore > 1.0f) {
        raw_age = 1;  // Middle
    } else {
        raw_age = 0;  // Young
    }
    age_history[smooth_idx] = raw_age;
    
    // Majority vote for heart age (most common value in window)
    uint8_t age_counts[3] = {0, 0, 0};
    for (int i = 0; i < SMOOTH_WINDOW; i++) {
        if (age_history[i] < 3) age_counts[age_history[i]]++;
    }
    if (age_counts[2] >= (SMOOTH_WINDOW / 2 + 1)) {
        results.heart_age = 2;
    } else if (age_counts[1] >= (SMOOTH_WINDOW / 2 + 1)) {
        results.heart_age = 1;
    } else {
        results.heart_age = 0;
    }
    
    // Advance smoothing index (shared by all smoothed metrics)
    smooth_idx = (smooth_idx + 1) % SMOOTH_WINDOW;
}

void SystemClock_Config(void) {
    RCC->CR |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY));
    
    FLASH->ACR = FLASH_ACR_LATENCY_2WS | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN;
    
    RCC->PLLCFGR = (RCC_PLLCFGR_PLLSRC_HSI |
                    (168 << RCC_PLLCFGR_PLLN_Pos) |
                    (16 << RCC_PLLCFGR_PLLM_Pos) |
                    (0 << RCC_PLLCFGR_PLLP_Pos) |
                    (7 << RCC_PLLCFGR_PLLQ_Pos));
    
    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY));
    
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
    
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

void UART_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    
    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    GPIOA->MODER |= (2 << GPIO_MODER_MODER2_Pos) | (2 << GPIO_MODER_MODER3_Pos);
    
    GPIOA->AFR[0] &= ~((0xF << GPIO_AFRL_AFSEL2_Pos) | (0xF << GPIO_AFRL_AFSEL3_Pos));
    GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos);
    
    // Configure USART2 for 9600 baud @ 42 MHz APB1 clock
    // USARTDIV = 42MHz / 9600 = 4375 = 0x1117
    USART2->BRR = 0x1117;
    
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void UART_SendString(const char* str) {
    while(*str) {
        while(!(USART2->SR & USART_SR_TXE));
        USART2->DR = *str++;
    }
}

void UART_SendInt(int val) {
    char buf[12];
    int i = 0;
    if (val < 0) { UART_SendString("-"); val = -val; }
    if (val == 0) { UART_SendString("0"); return; }
    while (val > 0) { buf[i++] = '0' + (val % 10); val /= 10; }
    while (i > 0) { char c[2] = {buf[--i], 0}; UART_SendString(c); }
}

void UART_SendFloat(float val) {
    // Send float with 3 decimal places
    if (val < 0) { UART_SendString("-"); val = -val; }
    int integer_part = (int)val;
    int decimal_part = (int)((val - integer_part) * 1000);
    UART_SendInt(integer_part);
    UART_SendString(".");
    // Pad with leading zeros for decimal
    if (decimal_part < 100) UART_SendString("0");
    if (decimal_part < 10) UART_SendString("0");
    UART_SendInt(decimal_part);
}

void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms * 10500; i++) {
        __NOP();
    }
}
#include "display_ui.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "stm32f4xx.h"
#include <stdio.h>

#define BTN_PORT GPIOC
#define BTN_PIN  13

#define DEBOUNCE_MS 40

extern void delay_ms(uint32_t ms);

static void draw_text(uint8_t x, uint8_t y, const char *s, const SSD1306_Font_t font) {
    ssd1306_SetCursor(x, y);
    ssd1306_WriteString((char*)s, font, White);
}

static const char* rhythm_label(uint8_t rhythm) {
    return rhythm ? "Irreg" : "Norm";
}

static const char* stress_label(int stress) {
    if (stress < 20) return "Low";
    if (stress < 50) return "Med";
    return "High";
}

static const char* resp_label(uint8_t copd) {
    return copd ? "Elev" : "Norm";
}

static const char* heart_label(uint8_t age) {
    switch (age) {
        case 0: return "Norm";
        case 1: return "Mild";
        case 2: return "High";
        default: return "Unk";
    }
}

void Display_UI_Init(void) {
    ssd1306_Init();
    ssd1306_Fill(Black);
    ssd1306_UpdateScreen();
}

void Display_UI_ShowBoot(void) {
    ssd1306_Fill(Black);

    draw_text(0, 0,  "EdgeCardia", Font_11x18);
    draw_text(0, 24, "ECG Monitor", Font_7x10);
    draw_text(0, 40, "Starting...", Font_7x10);

    ssd1306_UpdateScreen();
}

void Display_UI_ShowNoLeads(void) {
    ssd1306_Fill(Black);

    draw_text(0, 0,  "EdgeCardia", Font_11x18);
    draw_text(0, 24, "LEADS OFF", Font_11x18);
    draw_text(0, 46, "Check electrodes", Font_6x8);

    ssd1306_UpdateScreen();
}

void Display_UI_ShowStatus(
    int bpm,
    uint8_t rhythm,
    int stress,
    uint8_t copd,
    uint8_t age,
    uint8_t leads_ok,
    uint8_t wifi_ok
) {
    char line[32];

    ssd1306_Fill(Black);

    snprintf(line, sizeof(line), "BPM: %d", bpm);
    draw_text(0, 0, line, Font_11x18);

    snprintf(line, sizeof(line), "Rhythm: %s", rhythm_label(rhythm));
    draw_text(0, 22, line, Font_7x10);

    snprintf(line, sizeof(line), "Stress: %s", stress_label(stress));
    draw_text(0, 34, line, Font_7x10);

    snprintf(line, sizeof(line), "Resp:%s Heart:%s",
             resp_label(copd), heart_label(age));
    draw_text(0, 46, line, Font_6x8);

    snprintf(line, sizeof(line), "Lead:%s WiFi:%s",
             leads_ok ? "ON" : "OFF",
             wifi_ok ? "OK" : "DOWN");
    draw_text(0, 56, line, Font_6x8);

    ssd1306_UpdateScreen();
}

void Display_UI_ShowWave(
    const float *samples,
    uint16_t count,
    uint16_t head,
    int bpm,
    uint8_t leads_ok
) {
    char line[24];

    if (count < 2) return;

    ssd1306_Fill(Black);

    snprintf(line, sizeof(line), "BPM:%d", bpm);
    draw_text(0, 0, line, Font_11x18);

    snprintf(line, sizeof(line), "%s", leads_ok ? "Lead ON" : "Lead OFF");
    draw_text(78, 4, line, Font_6x8);

    const uint8_t y_min = 22;
    const uint8_t y_max = 63;
    const uint8_t center_y = 43;

    for (uint8_t x = 0; x < 128; x += 4) {
        ssd1306_DrawPixel(x, center_y, White);
    }

    uint16_t display_points = 128;

    if (count < display_points) {
        display_points = count;
    }

    float smooth[128];

    for (uint16_t x = 0; x < display_points; x++) {
        uint16_t raw_index = (head + x) % count;

        float s = samples[raw_index];

        if (s > 1.0f) s = 1.0f;
        if (s < -1.0f) s = -1.0f;

        if (x == 0) {
            smooth[x] = s;
        } else {
            smooth[x] = 0.82f * smooth[x - 1] + 0.18f * s;
        }
    }

    float min_val = smooth[0];
    float max_val = smooth[0];

    for (uint16_t i = 1; i < display_points; i++) {
        if (smooth[i] < min_val) min_val = smooth[i];
        if (smooth[i] > max_val) max_val = smooth[i];
    }

    float range = max_val - min_val;

    if (range < 0.25f) {
        range = 0.25f;
    }

    float mid = (max_val + min_val) * 0.5f;
    float scale = 30.0f / range;

    if (scale > 30.0f) {
        scale = 30.0f;
    }

    if (scale < 8.0f) {
        scale = 8.0f;
    }

    for (uint16_t x = 0; x < display_points - 1; x++) {
        float s1 = smooth[x];
        float s2 = smooth[x + 1];

        int y1 = center_y - (int)((s1 - mid) * scale);
        int y2 = center_y - (int)((s2 - mid) * scale);

        if (y1 < y_min) y1 = y_min;
        if (y1 > y_max) y1 = y_max;

        if (y2 < y_min) y2 = y_min;
        if (y2 > y_max) y2 = y_max;

        ssd1306_Line(x, y1, x + 1, y2, White);
    }

    ssd1306_UpdateScreen();
}

void Display_Button_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    BTN_PORT->MODER &= ~(3U << (BTN_PIN * 2));

    BTN_PORT->PUPDR &= ~(3U << (BTN_PIN * 2));
    BTN_PORT->PUPDR |=  (1U << (BTN_PIN * 2));
}

uint8_t Display_Button_Pressed(void) {
    static uint8_t last_state = 1;

    uint8_t current = (BTN_PORT->IDR & (1U << BTN_PIN)) ? 1 : 0;

    if (last_state == 1 && current == 0) {
        delay_ms(DEBOUNCE_MS);

        current = (BTN_PORT->IDR & (1U << BTN_PIN)) ? 1 : 0;

        if (current == 0) {
            while (((BTN_PORT->IDR & (1U << BTN_PIN)) ? 1 : 0) == 0) {
            }

            delay_ms(DEBOUNCE_MS);
            last_state = 1;
            return 1;
        }
    }

    last_state = current;
    return 0;
}
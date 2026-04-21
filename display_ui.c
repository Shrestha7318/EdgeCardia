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

    snprintf(line, sizeof(line), "ECG BPM:%d", bpm);
    draw_text(0, 0, line, Font_6x8);

    snprintf(line, sizeof(line), "Lead:%s", leads_ok ? "ON" : "OFF");
    draw_text(80, 0, line, Font_6x8);

    const uint8_t y_min = 12;
    const uint8_t y_max = 63;
    const uint8_t plot_h = y_max - y_min;

    for (uint8_t x = 0; x < 128; x += 2) {
        ssd1306_DrawPixel(x, y_min + plot_h / 2, White);
    }

    uint16_t plot_count = (count > 128) ? 128 : count;

    for (uint16_t x = 0; x < (plot_count - 1); x++) {
        uint16_t i1 = (head + x) % count;
        uint16_t i2 = (head + x + 1) % count;

        float s1 = samples[i1];
        float s2 = samples[i2];

        if (s1 > 1.2f) s1 = 1.2f;
        if (s1 < -1.2f) s1 = -1.2f;
        if (s2 > 1.2f) s2 = 1.2f;
        if (s2 < -1.2f) s2 = -1.2f;

        uint8_t y1 = (uint8_t)(y_min + ((1.2f - s1) * plot_h / 2.4f));
        uint8_t y2 = (uint8_t)(y_min + ((1.2f - s2) * plot_h / 2.4f));

        if (y1 > y_max) y1 = y_max;
        if (y2 > y_max) y2 = y_max;

        ssd1306_Line(x, y1, x + 1, y2, White);
    }

    ssd1306_UpdateScreen();
}

void Display_Button_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    // Input mode
    BTN_PORT->MODER &= ~(3U << (BTN_PIN * 2));

    // Pull-up enabled
    BTN_PORT->PUPDR &= ~(3U << (BTN_PIN * 2));
    BTN_PORT->PUPDR |=  (1U << (BTN_PIN * 2));
}

uint8_t Display_Button_Pressed(void) {
    static uint8_t last_state = 1;

    uint8_t current = (BTN_PORT->IDR & (1U << BTN_PIN)) ? 1 : 0;

    // Active low press detect on falling edge
    if (last_state == 1 && current == 0) {
        delay_ms(DEBOUNCE_MS);

        current = (BTN_PORT->IDR & (1U << BTN_PIN)) ? 1 : 0;
        if (current == 0) {
            // Wait for release so one press = one toggle
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
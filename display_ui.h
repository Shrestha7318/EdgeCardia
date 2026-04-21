#ifndef DISPLAY_UI_H
#define DISPLAY_UI_H

#include <stdint.h>

typedef enum {
    DISPLAY_MODE_STATUS = 0,
    DISPLAY_MODE_WAVE   = 1
} DisplayMode_t;

void Display_UI_Init(void);
void Display_UI_ShowBoot(void);
void Display_UI_ShowNoLeads(void);

void Display_UI_ShowStatus(
    int bpm,
    uint8_t rhythm,
    int stress,
    uint8_t copd,
    uint8_t age,
    uint8_t leads_ok,
    uint8_t wifi_ok
);

void Display_UI_ShowWave(
    const float *samples,
    uint16_t count,
    uint16_t head,
    int bpm,
    uint8_t leads_ok
);

void Display_Button_Init(void);
uint8_t Display_Button_Pressed(void);

#endif
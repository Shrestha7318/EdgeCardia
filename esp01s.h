#ifndef ESP01S_H
#define ESP01S_H

#include <stdint.h>

void ESP_Init(void);
void ESP_FlushRX(void);

void ESP_SendChar(char c);
void ESP_SendString(const char *s);

uint8_t ESP_ReadLine(char *buf, uint16_t maxlen, uint32_t timeout_ms);
uint8_t ESP_WaitFor(const char *token, uint32_t timeout_ms);

uint8_t ESP_Test(void);
uint8_t ESP_DisableEcho(void);
uint8_t ESP_SetStationMode(void);
uint8_t ESP_JoinAP(const char *ssid, const char *password);

uint8_t ESP_StartTCP(const char *host, uint16_t port);
uint8_t ESP_Close(void);

uint8_t ESP_SendRaw(const char *payload);
uint8_t ESP_HTTP_PostMetrics(
    const char *host,
    uint16_t port,
    const char *path,
    int bpm,
    uint8_t rhythm,
    int stress,
    uint8_t copd,
    uint8_t age,
    uint8_t leads_ok
);

#endif
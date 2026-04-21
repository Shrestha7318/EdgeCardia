#include "esp01s.h"
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

/*
 * Assumes:
 * USART1 on PA9(TX), PA10(RX)
 * APB2 = 84 MHz
 * Baud = 115200
 *
 * BRR for 84MHz / 115200 ~= 0x02D9
 *
 * If your ESP is configured for 9600 instead,
 * change BRR to 0x222E.
 */

extern void delay_ms(uint32_t ms);

#define ESP_USART USART1
#define ESP_BRR_115200 0x02D9
#define ESP_BRR_9600   0x222E

static int str_contains(const char *haystack, const char *needle) {
    return strstr(haystack, needle) != 0;
}

void ESP_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // PA9 / PA10 alternate function
    GPIOA->MODER &= ~((3U << (9 * 2)) | (3U << (10 * 2)));
    GPIOA->MODER |=  ((2U << (9 * 2)) | (2U << (10 * 2)));

    GPIOA->OSPEEDR |= ((3U << (9 * 2)) | (3U << (10 * 2)));
    GPIOA->PUPDR &= ~((3U << (9 * 2)) | (3U << (10 * 2)));
    GPIOA->PUPDR |=  ((1U << (10 * 2)));  // RX pull-up

    GPIOA->AFR[1] &= ~((0xFU << ((9 - 8) * 4)) | (0xFU << ((10 - 8) * 4)));
    GPIOA->AFR[1] |=  ((7U   << ((9 - 8) * 4)) | (7U   << ((10 - 8) * 4)));

    ESP_USART->CR1 = 0;
    ESP_USART->BRR = ESP_BRR_115200;
    ESP_USART->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

    delay_ms(500);
}

void ESP_FlushRX(void) {
    while (ESP_USART->SR & USART_SR_RXNE) {
        volatile uint8_t dummy = (uint8_t)ESP_USART->DR;
        (void)dummy;
    }
}

void ESP_SendChar(char c) {
    while (!(ESP_USART->SR & USART_SR_TXE)) {
    }
    ESP_USART->DR = (uint8_t)c;
}

void ESP_SendString(const char *s) {
    while (*s) {
        ESP_SendChar(*s++);
    }
}

uint8_t ESP_ReadLine(char *buf, uint16_t maxlen, uint32_t timeout_ms) {
    uint16_t idx = 0;

    while (timeout_ms--) {
        if (ESP_USART->SR & USART_SR_RXNE) {
            char c = (char)ESP_USART->DR;

            if (c == '\r') {
                continue;
            }

            if (c == '\n') {
                if (idx == 0) {
                    continue;
                }
                break;
            }

            if (idx < (maxlen - 1)) {
                buf[idx++] = c;
            }
        } else {
            delay_ms(1);
        }
    }

    buf[idx] = '\0';
    return (idx > 0) ? 1 : 0;
}

uint8_t ESP_WaitFor(const char *token, uint32_t timeout_ms) {
    char line[192];

    while (timeout_ms > 0) {
        if (ESP_ReadLine(line, sizeof(line), 50)) {
            if (str_contains(line, token)) {
                return 1;
            }
            if (str_contains(line, "ERROR") || str_contains(line, "FAIL")) {
                return 0;
            }
        }

        if (timeout_ms >= 50) timeout_ms -= 50;
        else timeout_ms = 0;
    }

    return 0;
}

uint8_t ESP_Test(void) {
    ESP_FlushRX();
    ESP_SendString("AT\r\n");
    return ESP_WaitFor("OK", 1000);
}

uint8_t ESP_DisableEcho(void) {
    ESP_FlushRX();
    ESP_SendString("ATE0\r\n");
    return ESP_WaitFor("OK", 1000);
}

uint8_t ESP_SetStationMode(void) {
    ESP_FlushRX();
    ESP_SendString("AT+CWMODE=1\r\n");
    return ESP_WaitFor("OK", 1000);
}

uint8_t ESP_JoinAP(const char *ssid, const char *password) {
    char cmd[160];

    ESP_FlushRX();
    snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);
    ESP_SendString(cmd);

    // Joining can take several seconds
    return ESP_WaitFor("OK", 15000);
}

uint8_t ESP_StartTCP(const char *host, uint16_t port) {
    char cmd[160];

    ESP_FlushRX();
    snprintf(cmd, sizeof(cmd), "AT+CIPSTART=\"TCP\",\"%s\",%u\r\n", host, port);
    ESP_SendString(cmd);

    if (ESP_WaitFor("OK", 5000)) return 1;
    if (ESP_WaitFor("CONNECT", 5000)) return 1;
    if (ESP_WaitFor("ALREADY CONNECTED", 2000)) return 1;

    return 0;
}

uint8_t ESP_Close(void) {
    ESP_FlushRX();
    ESP_SendString("AT+CIPCLOSE\r\n");
    return ESP_WaitFor("OK", 2000);
}

uint8_t ESP_SendRaw(const char *payload) {
    char cmd[32];
    uint16_t len = (uint16_t)strlen(payload);

    ESP_FlushRX();
    snprintf(cmd, sizeof(cmd), "AT+CIPSEND=%u\r\n", len);
    ESP_SendString(cmd);

    if (!ESP_WaitFor(">", 3000)) {
        return 0;
    }

    ESP_SendString(payload);

    if (ESP_WaitFor("SEND OK", 5000)) return 1;
    return 0;
}

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
) {
    char body[160];
    char req[512];

    snprintf(body, sizeof(body),
             "{\"bpm\":%d,\"rhythm\":%u,\"stress\":%d,\"copd\":%u,\"age\":%u,\"leads\":%u}",
             bpm, rhythm, stress, copd, age, leads_ok);

    snprintf(req, sizeof(req),
             "POST %s HTTP/1.1\r\n"
             "Host: %s:%u\r\n"
             "Content-Type: application/json\r\n"
             "Content-Length: %u\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s",
             path, host, port, (unsigned)strlen(body), body);

    if (!ESP_StartTCP(host, port)) {
        return 0;
    }

    if (!ESP_SendRaw(req)) {
        ESP_Close();
        return 0;
    }

    if (!ESP_WaitFor("200", 5000)) {
        ESP_Close();
        return 0;
    }

    ESP_Close();
    return 1;
}
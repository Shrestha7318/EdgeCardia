/*
 * esp01s.c
 * ESP-01S AT driver for STM32F446RE
 *
 * USART1:
 * PA9  = TX to ESP RX
 * PA10 = RX from ESP TX
 *
 * Baud: 115200
 */

#include "esp01s.h"
#include "stm32f4xx.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

extern void delay_ms(uint32_t ms);

#define ESP_USART USART1

#define ESP_BRR_115200 0x02D9
#define ESP_BRR_9600   0x222E

#define ESP_RX_BUFFER_SIZE 512

static char esp_rx_buffer[ESP_RX_BUFFER_SIZE];

static int contains(const char *text, const char *key) {
    return strstr(text, key) != 0;
}

void ESP_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    GPIOA->MODER &= ~((3U << (9 * 2)) | (3U << (10 * 2)));
    GPIOA->MODER |=  ((2U << (9 * 2)) | (2U << (10 * 2)));

    GPIOA->OSPEEDR |= ((3U << (9 * 2)) | (3U << (10 * 2)));

    GPIOA->PUPDR &= ~((3U << (9 * 2)) | (3U << (10 * 2)));
    GPIOA->PUPDR |=  (1U << (10 * 2));

    GPIOA->AFR[1] &= ~((0xFU << ((9 - 8) * 4)) |
                       (0xFU << ((10 - 8) * 4)));

    GPIOA->AFR[1] |=  ((7U << ((9 - 8) * 4)) |
                       (7U << ((10 - 8) * 4)));

    ESP_USART->CR1 = 0;
    ESP_USART->BRR = ESP_BRR_115200;
    ESP_USART->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

    delay_ms(1000);
}

void ESP_FlushRX(void) {
    while (ESP_USART->SR & USART_SR_RXNE) {
        volatile uint8_t dummy = ESP_USART->DR;
        (void)dummy;
    }
}

void ESP_SendChar(char c) {
    while (!(ESP_USART->SR & USART_SR_TXE));
    ESP_USART->DR = (uint8_t)c;
}

void ESP_SendString(const char *s) {
    while (*s) {
        ESP_SendChar(*s++);
    }
}

static void ESP_ReadAvailable(char *buf, uint16_t maxlen, uint32_t timeout_ms) {
    uint16_t idx = 0;

    if (maxlen == 0) {
        return;
    }

    memset(buf, 0, maxlen);

    while (timeout_ms--) {
        while (ESP_USART->SR & USART_SR_RXNE) {
            char c = (char)ESP_USART->DR;

            if (idx < maxlen - 1) {
                buf[idx++] = c;
                buf[idx] = '\0';
            }
        }

        delay_ms(1);
    }
}

uint8_t ESP_ReadLine(char *buf, uint16_t maxlen, uint32_t timeout_ms) {
    uint16_t idx = 0;

    if (maxlen == 0) {
        return 0;
    }

    memset(buf, 0, maxlen);

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

                buf[idx] = '\0';
                return 1;
            }

            if (idx < maxlen - 1) {
                buf[idx++] = c;
            }
        } else {
            delay_ms(1);
        }
    }

    buf[idx] = '\0';
    return idx > 0;
}

uint8_t ESP_WaitFor(const char *token, uint32_t timeout_ms) {
    memset(esp_rx_buffer, 0, sizeof(esp_rx_buffer));

    uint16_t idx = 0;

    while (timeout_ms--) {
        while (ESP_USART->SR & USART_SR_RXNE) {
            char c = (char)ESP_USART->DR;

            if (idx < ESP_RX_BUFFER_SIZE - 1) {
                esp_rx_buffer[idx++] = c;
                esp_rx_buffer[idx] = '\0';
            }

            if (contains(esp_rx_buffer, token)) {
                return 1;
            }

            if (contains(esp_rx_buffer, "ERROR") || contains(esp_rx_buffer, "FAIL")) {
                return 0;
            }
        }

        delay_ms(1);
    }

    return 0;
}

uint8_t ESP_SendCommand(const char *cmd, const char *expected, uint32_t timeout_ms) {
    ESP_FlushRX();
    delay_ms(50);

    ESP_SendString(cmd);

    return ESP_WaitFor(expected, timeout_ms);
}

uint8_t ESP_Test(void) {
    for (int i = 0; i < 5; i++) {
        ESP_FlushRX();
        delay_ms(100);

        ESP_SendString("AT\r\n");

        if (ESP_WaitFor("OK", 2000)) {
            return 1;
        }

        delay_ms(500);
    }

    return 0;
}

uint8_t ESP_DisableEcho(void) {
    for (int i = 0; i < 3; i++) {
        if (ESP_SendCommand("ATE0\r\n", "OK", 2000)) {
            return 1;
        }

        delay_ms(300);
    }

    return 0;
}

uint8_t ESP_SetStationMode(void) {
    for (int i = 0; i < 3; i++) {
        if (ESP_SendCommand("AT+CWMODE=1\r\n", "OK", 3000)) {
            return 1;
        }

        delay_ms(300);
    }

    return 0;
}

uint8_t ESP_JoinAP(const char *ssid, const char *password) {
    char cmd[180];

    snprintf(cmd, sizeof(cmd),
             "AT+CWJAP=\"%s\",\"%s\"\r\n",
             ssid,
             password);

    for (int i = 0; i < 2; i++) {
        ESP_FlushRX();
        delay_ms(200);

        ESP_SendString(cmd);

        if (ESP_WaitFor("OK", 20000)) {
            return 1;
        }

        delay_ms(1000);
    }

    return 0;
}

uint8_t ESP_StartTCP(const char *host, uint16_t port) {
    char cmd[180];

    snprintf(cmd, sizeof(cmd),
             "AT+CIPSTART=\"TCP\",\"%s\",%u\r\n",
             host,
             port);

    ESP_FlushRX();
    delay_ms(100);

    ESP_SendString(cmd);

    if (ESP_WaitFor("CONNECT", 8000)) {
        return 1;
    }

    if (ESP_WaitFor("OK", 2000)) {
        return 1;
    }

    if (ESP_WaitFor("ALREADY CONNECTED", 2000)) {
        return 1;
    }

    return 0;
}

uint8_t ESP_Close(void) {
    ESP_FlushRX();
    delay_ms(50);

    ESP_SendString("AT+CIPCLOSE\r\n");

    if (ESP_WaitFor("OK", 2000)) {
        return 1;
    }

    if (ESP_WaitFor("CLOSED", 2000)) {
        return 1;
    }

    return 0;
}

uint8_t ESP_SendRaw(const char *payload) {
    char cmd[40];
    uint16_t len = (uint16_t)strlen(payload);

    snprintf(cmd, sizeof(cmd), "AT+CIPSEND=%u\r\n", len);

    ESP_FlushRX();
    delay_ms(100);

    ESP_SendString(cmd);

    if (!ESP_WaitFor(">", 5000)) {
        return 0;
    }

    delay_ms(100);
    ESP_SendString(payload);

    if (ESP_WaitFor("SEND OK", 8000)) {
        return 1;
    }

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
    char req[600];

    snprintf(body, sizeof(body),
             "{\"bpm\":%d,\"rhythm\":%u,\"stress\":%d,\"copd\":%u,\"age\":%u,\"leads\":%u}",
             bpm,
             rhythm,
             stress,
             copd,
             age,
             leads_ok);

    snprintf(req, sizeof(req),
             "POST %s HTTP/1.1\r\n"
             "Host: %s:%u\r\n"
             "Content-Type: application/json\r\n"
             "Content-Length: %u\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s",
             path,
             host,
             port,
             (unsigned)strlen(body),
             body);

    if (!ESP_StartTCP(host, port)) {
        return 0;
    }

    if (!ESP_SendRaw(req)) {
        ESP_Close();
        return 0;
    }

    /*
     * Your Node server replies with HTTP 200 OK.
     * We wait for either "200" or "OK".
     */
    if (ESP_WaitFor("200", 5000)) {
        ESP_Close();
        return 1;
    }

    ESP_Close();
    return 0;
}
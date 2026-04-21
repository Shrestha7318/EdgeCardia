/*
 * ecg_hardware.c
 * Reads ECG from AD8232 sensor connected to PA0 (ADC1_IN0)
 * Lead-off detection:
 *   LO- -> PC0
 *   LO+ -> PC1
 */

#include "ecg_hardware.h"
#include "stm32f4xx.h"

/**
 * Initialize ADC1 for reading ECG from PA0
 * and GPIOC pins for lead-off detection
 */
void ECG_Hardware_Init(void) {
    /* Enable clocks */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;   // GPIOA clock for PA0
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;   // GPIOC clock for PC0, PC1
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;    // ADC1 clock

    /* Configure PA0 as analog input */
    GPIOA->MODER &= ~(3U << (0 * 2));
    GPIOA->MODER |=  (3U << (0 * 2));      // Analog mode = 11
    GPIOA->PUPDR &= ~(3U << (0 * 2));      // No pull-up/pull-down

    /* Configure PC0 and PC1 as input for lead-off detection */
    GPIOC->MODER &= ~((3U << (0 * 2)) | (3U << (1 * 2)));   // Input mode = 00

    /* Optional but recommended: pull-down for stable logic when idle */
    GPIOC->PUPDR &= ~((3U << (0 * 2)) | (3U << (1 * 2)));
    GPIOC->PUPDR |=  ((2U << (0 * 2)) | (2U << (1 * 2)));   // Pull-down = 10

    /* Configure ADC1 */
    ADC1->CR1 = 0;    // 12-bit resolution
    ADC1->CR2 = 0;    // Reset basic control settings

    /*
     * Set sampling time for channel 0 (PA0 / ADC1_IN0)
     * 84 cycles @ 21 MHz ADC clock
     */
    ADC1->SMPR2 &= ~(7U << 0);
    ADC1->SMPR2 |=  (4U << 0);   // SMP0 = 100 => 84 cycles

    /* Configure regular sequence: 1 conversion, channel 0 */
    ADC1->SQR1 = 0;   // L[3:0] = 0000 => 1 conversion
    ADC1->SQR2 = 0;
    ADC1->SQR3 = 0;   // 1st conversion = channel 0

    /* Turn on ADC */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* Short stabilization delay */
    for (volatile int i = 0; i < 1000; i++) {
        __NOP();
    }
}

/**
 * Read single ECG sample from ADC
 *
 * @return ADC value (0-4095)
 */
uint16_t ECG_Hardware_GetSample(void) {
    /* Start conversion */
    ADC1->CR2 |= ADC_CR2_SWSTART;

    /* Wait for conversion complete */
    while (!(ADC1->SR & ADC_SR_EOC)) {
    }

    /* Read and return result */
    return (uint16_t)ADC1->DR;
}

/**
 * Check if electrodes are connected
 *
 * Wiring:
 *   LO- -> PC0
 *   LO+ -> PC1
 *
 * AD8232 behavior:
 *   If either LO+ or LO- is HIGH, leads are disconnected
 *
 * @return 1 if connected, 0 if disconnected
 */
uint8_t ECG_Hardware_LeadsConnected(void) {
    uint8_t lo_minus = (GPIOC->IDR & GPIO_IDR_ID0) ? 1U : 0U;  // PC0
    uint8_t lo_plus  = (GPIOC->IDR & GPIO_IDR_ID1) ? 1U : 0U;  // PC1

    /* Connected only if both are LOW */
    return (lo_plus == 0U && lo_minus == 0U) ? 1U : 0U;
}
/*
 * ecg_hardware.c
 * Reads ECG from AD8232 sensor connected to PA0 (ADC1_IN0)
 */

#include "ecg_hardware.h"
#include "stm32f4xx.h"

/**
 * Initialize ADC1 for reading ECG from PA0
 */
void ECG_Hardware_Init(void) {
    // Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // GPIOA clock
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;   // ADC1 clock
    
    // Configure PA0 as analog input
    GPIOA->MODER |= GPIO_MODER_MODER0;  // Analog mode (11)
    
    // Configure PB0 and PB1 as inputs for lead-off detection (optional)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1);  // Input mode
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR1);  // No pull-up/down
    
    // Configure ADC1
    ADC1->CR2 = 0;  // Reset
    ADC1->CR1 = 0;  // 12-bit resolution
    
    // Set sampling time for channel 0 (PA0)
    // 84 cycles @ 21MHz ADC clock = ~4us sampling time
    ADC1->SMPR2 = ADC_SMPR2_SMP0_2;  // 84 cycles
    
    // Configure sequence - channel 0 only
    ADC1->SQR3 = 0;  // Channel 0
    ADC1->SQR1 = 0;  // 1 conversion
    
    // Turn on ADC
    ADC1->CR2 |= ADC_CR2_ADON;
    
    // Wait for ADC to stabilize
    for(volatile int i = 0; i < 1000; i++);
}

/**
 * Read single ECG sample from ADC
 * 
 * @return ADC value (0-4095)
 */
uint16_t ECG_Hardware_GetSample(void) {
    // Start conversion
    ADC1->CR2 |= ADC_CR2_SWSTART;
    
    // Wait for conversion to complete
    while(!(ADC1->SR & ADC_SR_EOC));
    
    // Read and return result
    return ADC1->DR;
}

/**
 * Check if electrodes are connected
 * 
 * @return 1 if connected, 0 if disconnected
 */
uint8_t ECG_Hardware_LeadsConnected(void) {
    // Read LO+ and LO- pins (PB0 and PB1)
    // If either is HIGH, leads are disconnected
    uint8_t lo_plus = (GPIOB->IDR & GPIO_IDR_ID0) ? 1 : 0;
    uint8_t lo_minus = (GPIOB->IDR & GPIO_IDR_ID1) ? 1 : 0;
    
    // Return 1 if both are LOW (connected)
    return (lo_plus == 0 && lo_minus == 0) ? 1 : 0;
}
#include "i2c.h"

static void i2c_delay_small(void) {
    for (volatile int i = 0; i < 80; i++) {
        __NOP();
    }
}

void i2c_init(void) {
    // Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // PB8 = SCL, PB9 = SDA
    GPIOB->MODER &= ~((3U << (8 * 2)) | (3U << (9 * 2)));
    GPIOB->MODER |=  ((2U << (8 * 2)) | (2U << (9 * 2)));   // AF mode

    GPIOB->OTYPER |= (1U << 8) | (1U << 9);                 // Open-drain
    GPIOB->OSPEEDR |= ((3U << (8 * 2)) | (3U << (9 * 2)));  // High speed

    GPIOB->PUPDR &= ~((3U << (8 * 2)) | (3U << (9 * 2)));
    GPIOB->PUPDR |=  ((1U << (8 * 2)) | (1U << (9 * 2)));   // Pull-up

    GPIOB->AFR[1] &= ~((0xFU << ((8 - 8) * 4)) | (0xFU << ((9 - 8) * 4)));
    GPIOB->AFR[1] |=  ((4U   << ((8 - 8) * 4)) | (4U   << ((9 - 8) * 4))); // AF4 = I2C1

    // Reset I2C1
    I2C1->CR1 = I2C_CR1_SWRST;
    i2c_delay_small();
    I2C1->CR1 = 0;

    /*
     * Assumes APB1 = 42 MHz
     * Standard mode 100 kHz
     */
    I2C1->CR2   = 42;        // Peripheral clock frequency in MHz
    I2C1->CCR   = 210;       // 42MHz / (2*100kHz) = 210
    I2C1->TRISE = 43;        // 42MHz * 1000ns + 1 = 43

    I2C1->CR1 |= I2C_CR1_PE;
}

void I2C1_Start(void) {
    // Wait until bus not busy
    while (I2C1->SR2 & I2C_SR2_BUSY) {
    }

    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB)) {
    }
}

void I2C1_SendAddress(uint8_t address_8bit_write) {
    I2C1->DR = address_8bit_write;
    while (!(I2C1->SR1 & I2C_SR1_ADDR)) {
    }

    // Clear ADDR by reading SR1 then SR2
    volatile uint32_t temp = I2C1->SR1;
    (void)temp;
    temp = I2C1->SR2;
    (void)temp;
}

void I2C1_SendData(uint8_t data) {
    while (!(I2C1->SR1 & I2C_SR1_TXE)) {
    }
    I2C1->DR = data;
    while (!(I2C1->SR1 & I2C_SR1_BTF)) {
    }
}

void I2C1_Stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP;
    i2c_delay_small();
}

uint8_t I2C1_IsDeviceReady(uint8_t address_8bit_write) {
    uint32_t timeout = 200000;

    // Clear AF if previously set
    I2C1->SR1 &= ~I2C_SR1_AF;

    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB)) {
        if (--timeout == 0) return 0;
    }

    I2C1->DR = address_8bit_write;

    timeout = 200000;
    while (!(I2C1->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF))) {
        if (--timeout == 0) {
            I2C1->CR1 |= I2C_CR1_STOP;
            return 0;
        }
    }

    if (I2C1->SR1 & I2C_SR1_AF) {
        I2C1->SR1 &= ~I2C_SR1_AF;
        I2C1->CR1 |= I2C_CR1_STOP;
        return 0;
    }

    volatile uint32_t temp = I2C1->SR1;
    (void)temp;
    temp = I2C1->SR2;
    (void)temp;

    I2C1->CR1 |= I2C_CR1_STOP;
    return 1;
}
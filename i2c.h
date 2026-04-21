#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include "stm32f4xx.h"

void i2c_init(void);

void I2C1_Start(void);
void I2C1_Stop(void);
void I2C1_SendAddress(uint8_t address_8bit_write);
void I2C1_SendData(uint8_t data);
uint8_t I2C1_IsDeviceReady(uint8_t address_8bit_write);

#endif
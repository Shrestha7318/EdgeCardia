/*
 * ecg_hardware.h
 * Reads real ECG data from AD8232 sensor via ADC
 */

#ifndef ECG_HARDWARE_H
#define ECG_HARDWARE_H

#include <stdint.h>

// Function prototypes
void ECG_Hardware_Init(void);
uint16_t ECG_Hardware_GetSample(void);
uint8_t ECG_Hardware_LeadsConnected(void);

#endif // ECG_HARDWARE_H
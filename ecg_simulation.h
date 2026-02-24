/*
 * ecg_simulation.h
 * EdgeCardia - Fake ECG Data Generator
 * 
 * Generates realistic ECG waveforms for testing without hardware
 */

#ifndef ECG_SIMULATION_H
#define ECG_SIMULATION_H

#include <stdint.h>

// Simulation modes
typedef enum {
    ECG_SIM_NORMAL = 0,      // Normal sinus rhythm
    ECG_SIM_TACHYCARDIA,     // Fast heart rate
    ECG_SIM_BRADYCARDIA,     // Slow heart rate
    ECG_SIM_ARRHYTHMIA,      // Irregular rhythm
    ECG_SIM_VTACH            // Ventricular tachycardia
} ECG_SimMode_t;

// Function prototypes
void ECG_Simulation_Init(void);
void ECG_Simulation_SetMode(ECG_SimMode_t mode);
uint16_t ECG_Simulation_GetNextSample(void);
float ECG_Simulation_GetNextSampleFloat(void);

#endif // ECG_SIMULATION_H
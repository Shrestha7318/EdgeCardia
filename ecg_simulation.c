/*
 * ecg_simulation.c
 * EdgeCardia - Fake ECG Data Generator Implementation
 * 
 * Generates realistic ECG waveforms using mathematical models
 */

#include "ecg_simulation.h"
#include <math.h>

// Constants
#define SAMPLING_RATE 250.0f  // Hz
#define TWO_PI 6.28318530718f

// Simulation state
static uint32_t sample_count = 0;
static ECG_SimMode_t current_mode = ECG_SIM_NORMAL;
static float heart_rate = 75.0f;  // BPM
static float time_offset = 0.0f;

/**
 * Initialize ECG simulation
 */
void ECG_Simulation_Init(void) {
    sample_count = 0;
    current_mode = ECG_SIM_NORMAL;
    heart_rate = 75.0f;
    time_offset = 0.0f;
}

/**
 * Set simulation mode
 */
void ECG_Simulation_SetMode(ECG_SimMode_t mode) {
    current_mode = mode;
    
    switch(mode) {
        case ECG_SIM_NORMAL:
            heart_rate = 75.0f;
            break;
        case ECG_SIM_TACHYCARDIA:
            heart_rate = 120.0f;
            break;
        case ECG_SIM_BRADYCARDIA:
            heart_rate = 45.0f;
            break;
        case ECG_SIM_ARRHYTHMIA:
            heart_rate = 80.0f;  // Base rate, will add irregularity
            break;
        case ECG_SIM_VTACH:
            heart_rate = 180.0f;
            break;
    }
}

/**
 * Generate single PQRST complex (one heartbeat)
 * 
 * Mathematical model of ECG waveform:
 * - P wave: Small positive deflection (atrial depolarization)
 * - QRS complex: Large spike (ventricular depolarization)
 * - T wave: Positive deflection (ventricular repolarization)
 */
static float generate_ecg_waveform(float t) {
    float ecg = 0.0f;
    
    // Beat frequency (convert BPM to Hz)
    float beat_freq = heart_rate / 60.0f;
    
    // Add irregularity for arrhythmia
    if (current_mode == ECG_SIM_ARRHYTHMIA) {
        // Random variation in beat timing
        float noise = sinf(t * 0.5f) * 0.3f;
        beat_freq += noise;
    }
    
    // Calculate phase (0 to 2p per heartbeat)
    float phase = fmodf(t * beat_freq * TWO_PI, TWO_PI);
    
    // P-Wave (atrial depolarization)
    // Occurs at phase 0 to p/3
    if (phase < 1.047f) {  // p/3
        float p_phase = phase / 1.047f;
        ecg += 0.15f * sinf(p_phase * 3.14159f);
    }
    
    // PR Interval (isoelectric)
    // Phase p/3 to p/2
    // (no contribution)
    
    // QRS Complex (ventricular depolarization)
    // Occurs at phase p/2 to 3p/4
    if (phase > 1.57f && phase < 2.356f) {  // p/2 to 3p/4
        float qrs_phase = (phase - 1.57f) / 0.786f;
        
        // Q wave (small negative)
        if (qrs_phase < 0.2f) {
            ecg += -0.1f * sinf(qrs_phase * 15.7f);
        }
        // R wave (large positive)
        else if (qrs_phase < 0.5f) {
            ecg += 1.2f * sinf((qrs_phase - 0.2f) * 10.47f);
        }
        // S wave (small negative)
        else {
            ecg += -0.2f * sinf((qrs_phase - 0.5f) * 6.28f);
        }
        
        // Make QRS wider for abnormal rhythms
        if (current_mode == ECG_SIM_VTACH || current_mode == ECG_SIM_ARRHYTHMIA) {
            ecg *= 0.7f;  // Reduce amplitude
        }
    }
    
    // ST Segment (isoelectric)
    // Phase 3p/4 to p
    // (no contribution)
    
    // T-Wave (ventricular repolarization)
    // Occurs at phase p to 5p/3
    if (phase > 3.14159f && phase < 5.236f) {  // p to 5p/3
        float t_phase = (phase - 3.14159f) / 2.094f;
        ecg += 0.25f * sinf(t_phase * 3.14159f);
        
        // Inverted T-wave for some abnormal conditions
        if (current_mode == ECG_SIM_ARRHYTHMIA && (sample_count % 1000 < 500)) {
            ecg *= -0.5f;
        }
    }
    
    // Add small baseline noise
    float noise = sinf(t * 50.0f) * 0.02f;
    noise += sinf(t * 150.0f) * 0.01f;
    ecg += noise;
    
    return ecg;
}

/**
 * Get next ECG sample as float (-1.0 to 1.0)
 */
float ECG_Simulation_GetNextSampleFloat(void) {
    float t = sample_count / SAMPLING_RATE;
    float ecg = generate_ecg_waveform(t);
    
    sample_count++;
    
    // Clamp to valid range
    if (ecg > 1.0f) ecg = 1.0f;
    if (ecg < -1.0f) ecg = -1.0f;
    
    return ecg;
}

/**
 * Get next ECG sample as ADC value (0-4095)
 */
uint16_t ECG_Simulation_GetNextSample(void) {
    float ecg_float = ECG_Simulation_GetNextSampleFloat();
    
    // Convert from [-1, 1] to [0, 4095] (12-bit ADC)
    // Center at 2048, scale by 1500 (to avoid clipping)
    uint16_t adc_value = (uint16_t)(2048.0f + (ecg_float * 1500.0f));
    
    // Ensure within bounds
    if (adc_value > 4095) adc_value = 4095;
    if (adc_value < 0) adc_value = 0;
    
    return adc_value;
}
/*
 * thresholds.h
 */

#ifndef THRESHOLDS_H
#define THRESHOLDS_H

// COPD Detection
#define P_WAVE_COPD_THRESHOLD 0.25f

// Heart Age Detection
#define QRS_WIDTH_AGE_THRESHOLD 100.0f
#define QRS_WIDTH_ELDERLY 120.0f

// Stress Levels (SDNN)
#define SDNN_HIGH_STRESS 20.0f
#define SDNN_MODERATE_STRESS 40.0f
#define SDNN_NORMAL 60.0f
#define SDNN_RELAXED 80.0f

// Fatigue Levels (RMSSD)
#define RMSSD_HIGH_FATIGUE 15.0f
#define RMSSD_MODERATE_FATIGUE 30.0f
#define RMSSD_NORMAL 50.0f
#define RMSSD_WELL_RESTED 70.0f

// Heart Rate
#define BPM_BRADYCARDIA 60.0f
#define BPM_NORMAL_MIN 60.0f
#define BPM_NORMAL_MAX 100.0f
#define BPM_TACHYCARDIA 100.0f

#endif // THRESHOLDS_H

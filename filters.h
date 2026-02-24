/*
 * filters.h
 * Butterworth Filter Coefficients
 */

#ifndef FILTERS_H
#define FILTERS_H

// High-Pass Filter (>0.5Hz) - Remove baseline wander
#define HP_ORDER 2
extern const float b_hp[3];  // ? Make sure you have "extern"
extern const float a_hp[3];  // ? Make sure you have "extern"

// Low-Pass Filter (<40Hz) - Remove high-frequency noise
#define LP_ORDER 2
extern const float b_lp[3];  // ? Make sure you have "extern"
extern const float a_lp[3];  // ? Make sure you have "extern"

// Function declaration
float apply_biquad_filter(float input, const float b[3], const float a[3], float state[2]);

#endif // FILTERS_H
/*
 * filters.c
 */

#include "filters.h"

// Filter coefficients definitions
const float b_hp[3] = {0.9911535951f, -1.9823071902f, 0.9911535951f};
const float a_hp[3] = {1.0000000000f, -1.9822289298f, 0.9823854506f};

const float b_lp[3] = {0.1453238839f, 0.2906477678f, 0.1453238839f};
const float a_lp[3] = {1.0000000000f, -0.6710290908f, 0.2523246263f};

/**
 * Apply biquad IIR filter (Direct Form II)
 */
float apply_biquad_filter(float input, const float b[3], const float a[3], float state[2]) {
    float w0 = input - a[1] * state[0] - a[2] * state[1];
    float output = b[0] * w0 + b[1] * state[0] + b[2] * state[1];
    
    // Update state
    state[1] = state[0];
    state[0] = w0;
    
    return output;
}
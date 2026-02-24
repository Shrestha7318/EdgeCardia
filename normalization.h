/*
 * normalization.h
 */

#ifndef NORMALIZATION_H
#define NORMALIZATION_H

#define NUM_FEATURES 8

// Feature means
static const float feature_means[NUM_FEATURES] = {
    748.0000000000f,  // RR-Interval (ms),
    4.3728709967f,  // SDNN,
    4.3731424293f,  // RMSSD,
    110.2028774117f,  // QRS Width (ms),
    -0.1102065654f,  // P-Amplitude,
    -0.0255129841f, // T-Amplitude,
    0.4781423775f,  // R-Amplitude,
    0.1011082798f,  // Signal Energy
};

// Feature standard deviations
static const float feature_stds[NUM_FEATURES] = {
    1.0000000000f,  // RR-Interval (ms),
    0.7424314431f,  // SDNN,
    0.7422425872f,  // RMSSD,
    34.1184189499f,  // QRS Width (ms),
    0.2777946009f,  // P-Amplitude,
    0.1470618612f,  // T-Amplitude,
    0.1813782726f,  // R-Amplitude,
    0.0399830829f,  // Signal Energy
};

static inline void normalize_features(float features[NUM_FEATURES]) {
    for (int i = 0; i < NUM_FEATURES; i++) {
        features[i] = (features[i] - feature_means[i]) / feature_stds[i];
    }
}

#endif // NORMALIZATION_H

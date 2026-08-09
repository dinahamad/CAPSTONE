// // ============================================================
// // Wearable Stabilization Device for Tremors
// //
// // File: tremor_detection.h
// //
// // Description:
// // Real-time tremor detection using the calibrated
// // relative wrist pitch.
// //
// // Processing Pipeline:
// //
// // Relative Wrist Pitch
// //          │
// //          ▼
// // 4–8 Hz Butterworth Band-pass
// //          │
// //          ▼
// // Filtered Wrist Pitch
// //          │
// //     ┌────┴────┐
// //     ▼         ▼
// // Envelope     FFT Buffer
// // Detector
// //     │         │
// //     └────┬────┘
// //          ▼
// // Tremor Detection Logic
// //          │
// //     Monitoring / Tracking
// // ============================================================

// #pragma once

// #include <Arduino.h>

// enum TremorState
// {
//     MONITORING = 0,
//     TREMOR_DETECTED
// };

// struct TremorEstimate
// {
//     float handPitch;

//     float forearmPitch;

//     float wristPitch;

//     float tremorPitch;

//     float envelope;

//     float frequency;

//     float confidence;

//     TremorState state;
// };

// void initializeTremorDetection();
// void updateTremorDetection(
//     float handPitch,
//     float forearmPitch,
//     float wristPitch
// );

// void updateBandPass(float sample);
// void updateEnvelope();
// void updatePeakFrequency();
// void updateStateMachine();

// float getRawPitch();
// float getFilteredPitch();
// float getEnvelope();
// float getDominantFrequency();
// float getDetectionConfidence();

// TremorState getTremorState();
// bool tremorDetected();

// const TremorEstimate& getTremorEstimate();

// void printTremorData();



// ============================================================
// Wearable Stabilization Device for Tremors
//
// File: tremor_detection.h
//
// Description:
// Real-time tremor detection using the calibrated
// relative wrist pitch.
//
// Processing Pipeline:
//
// Relative Wrist Pitch
//          │
//          ▼
// 4–8 Hz Butterworth Band-pass
//          │
//          ▼
// Tremor Pitch
//          │
//     ┌────┴────┐
//     ▼         ▼
// Envelope   Peak Detector
// Detector   (Frequency)
//     │         │
//     └────┬────┘
//          ▼
// Tremor Detection Logic
//          │
// Monitoring / Tremor Detected
// ============================================================

#pragma once

#include <Arduino.h>

// ============================================================
// TREMOR STATE
// ============================================================

enum TremorState
{
    MONITORING = 0,
    TREMOR_DETECTED
};

// ============================================================
// TREMOR ESTIMATE
// ============================================================

struct TremorEstimate
{
    // Raw orientation
    float handPitch;
    float forearmPitch;
    float wristPitch;

    // Band-pass filtered tremor
    float tremorPitch;

    // Envelope amplitude
    float envelope;

    // Estimated tremor frequency
    float frequency;

    // Detection confidence (%)
    float confidence;

    // Current detector state
    TremorState state;
};

// ============================================================
// INITIALIZATION
// ============================================================

void initializeTremorDetection();

// ============================================================
// UPDATE PIPELINE
// ============================================================

void updateTremorDetection(
    float handPitch,
    float forearmPitch,
    float wristPitch);

// ============================================================
// INTERNAL PROCESSING
// ============================================================

void updateBandPass(float sample);

void updateEnvelope();

void updatePeakFrequency();

void updateStateMachine();

// ============================================================
// GETTERS
// ============================================================

float getRawPitch();

float getFilteredPitch();

float getEnvelope();

float getDominantFrequency();

float getDetectionConfidence();

TremorState getTremorState();

bool tremorDetected();

const TremorEstimate& getTremorEstimate();

// ============================================================
// DEBUG
// ============================================================

void printTremorData();
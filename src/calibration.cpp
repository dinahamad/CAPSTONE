// ============================================================
// Wearable Stabilization Device for Tremors
//
// File: calibration.cpp
//
// Description:
// Stores and applies the neutral wrist position.
//
// Authors:
// - Didi Dimitrova
// ============================================================

#include "calibration.h"
#include "orientation.h"

// ============================================================
// GLOBAL VARIABLE
// ============================================================

static float neutralPitch = 0.0f;

// ============================================================
// CALIBRATE
// ============================================================

void calibrate()
{
    // -----------------------------------------
    // Let complementary filter settle
    // -----------------------------------------

    for (int i = 0; i < 100; i++)
    {
        updateOrientation(HAND_IMU);
        updateOrientation(FOREARM_IMU);

        delay(10);
    }

    // -----------------------------------------
    // Average neutral wrist position
    // -----------------------------------------

    const int samples = 50;
    float sum = 0.0f;

    Serial.println("Calibrating... Keep wrist still.");

    for (int i = 0; i < samples; i++)
    {
        updateOrientation(HAND_IMU);
        updateOrientation(FOREARM_IMU);

        sum += getRelativePitch();

        delay(10);
    }

    neutralPitch = sum / samples;

    Serial.print("Neutral Pitch Saved: ");
    Serial.println(neutralPitch, 2);
}

// ============================================================
// GET CALIBRATED PITCH
// ============================================================

float getCalibratedPitch()
{
    return getRelativePitch() - neutralPitch;
}

float getNeutralPitch()
{
    return neutralPitch;
}
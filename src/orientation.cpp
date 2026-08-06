// ============================================================
// Wearable Stabilization Device for Tremors
//
// File: orientation.cpp
//
// Description:
// Calculates the pitch angle of each IMU using a
// complementary filter.
//
// Authors:
// - Didi Dimitrova
// ============================================================

#include "orientation.h"

#include <math.h>

// ============================================================
// ACCELEROMETER PITCH
// ============================================================

float calculateAccelPitch(float ax, float ay, float az)
{
    return atan2(-ax, sqrt(ay * ay + az * az)) * RAD_TO_DEG;
}

// ============================================================
// UPDATE ORIENTATION
// ============================================================

bool updateOrientation(uint8_t index)
{
    // Read newest IMU data
    if (!readIMU(index))
    {
        return false;
    }

    // Pitch from accelerometer
    float accelPitch =
        calculateAccelPitch(
            imuData[index].ax,
            imuData[index].ay,
            imuData[index].az);

    // Integrate gyro
    float gyroPitch =
        imuData[index].pitch +
        imuData[index].gy * DT;

    // Complementary filter
    imuData[index].pitch =
        ALPHA * gyroPitch +
        (1.0f - ALPHA) * accelPitch;

    return true;
}

// ============================================================
// RELATIVE WRIST PITCH
// ============================================================

float getRelativePitch()
{
    return imuData[HAND_IMU].pitch -
           imuData[FOREARM_IMU].pitch;
}

// ============================================================
// RELATIVE WRIST PITCH RATE
// ============================================================

float getRelativePitchRate()
{
    return imuData[HAND_IMU].gy -
           imuData[FOREARM_IMU].gy;
}
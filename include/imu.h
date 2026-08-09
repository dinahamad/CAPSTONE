// ============================================================
// Wearable Stabilization Device for Tremors
//
// File: imu.h
//
// Description:
// Initializes and reads the two ICM-20948 IMUs.
//
// ============================================================

#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <ICM_20948.h>

#include "pins.h"

// ============================================================
// IMU DATA STRUCTURE
// ============================================================

// Stores the latest sensor measurements and orientation estimate for one IMU.
struct IMUData
{
    float ax;
    float ay;
    float az;

    float gx;
    float gy;
    float gz;

    // For Madgwick Filter
    // Quaternion orientation
    float qw = 1.0f;
    float qx = 0.0f;
    float qy = 0.0f;
    float qz = 0.0f;

    // Keep pitch for debugging/dashboard
    float pitch = 0.0f;
};

// ============================================================
// GLOBAL VARIABLES
// ============================================================

// Provides access to the ICM-20948 objects for both IMUs.
extern ICM_20948_SPI imu[NUM_IMUS];

// Provides access to the stored sensor and orientation data for both IMUs.
extern IMUData imuData[NUM_IMUS];

// ============================================================
// FUNCTIONS
// ============================================================

// Initializes the SPI bus and connects both the hand and forearm IMUs.
bool initializeIMUs();

// Reads the latest accelerometer and gyroscope data from the selected IMU.
bool readIMU(uint8_t index);
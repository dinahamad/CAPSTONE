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

struct IMUData
{
    float ax;
    float ay;
    float az;

    float gx;
    float gy;
    float gz;

    float pitch;
};

// ============================================================
// GLOBAL VARIABLES
// ============================================================

extern ICM_20948_SPI imu[NUM_IMUS];
extern IMUData imuData[NUM_IMUS];

// ============================================================
// FUNCTIONS
// ============================================================

bool initializeIMUs();

bool readIMU(uint8_t index);
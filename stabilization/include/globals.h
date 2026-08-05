// Didi Code
#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "ICM_20948.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "pins.h"

// ============================================================
// IMU ARRAYS
// ============================================================

extern const char* IMU_NAMES[NUM_IMUS];
extern const int CS_PINS[NUM_IMUS];

// ============================================================
// IMU OBJECTS
// ============================================================

extern ICM_20948_SPI imu[NUM_IMUS];
extern bool imuConnected[NUM_IMUS];

// ============================================================
// SYSTEM STATUS
// ============================================================

extern bool systemRunning;
extern bool calibrationComplete;
extern bool bleConnected;

// ============================================================
// BLE OBJECTS
// ============================================================

extern BLEServer* pServer;

extern BLECharacteristic* dataCharacteristic;
extern BLECharacteristic* commandCharacteristic;
extern BLECharacteristic* statusCharacteristic;

// ============================================================
// TIMING
// ============================================================

extern uint64_t startTime_us;
extern uint64_t lastTimestamp_us[NUM_IMUS];

// ============================================================
// ORIENTATION
// ============================================================

extern float orientation[NUM_IMUS][3];
extern float relativeOrientation[3];
extern float initialRelativeOrientation[3];

// ============================================================
// CALIBRATION
// ============================================================

extern float gyroBias[NUM_IMUS][3];
extern float accelBias[NUM_IMUS][3];

// ============================================================
// SERVOS
// ============================================================

extern bool topServoConnected;
extern bool bottomServoConnected;

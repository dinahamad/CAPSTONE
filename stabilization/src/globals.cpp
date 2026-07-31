// Didi Code

#include "globals.h"

const char* IMU_NAMES[NUM_IMUS] = {
    "HAND",
    "FOREARM"
};

const int CS_PINS[NUM_IMUS] = {
    CS_HAND,
    CS_FOREARM
};

ICM_20948_SPI imu[NUM_IMUS];

bool imuConnected[NUM_IMUS] = {
    false,
    false
};

bool systemRunning = false;
bool calibrationComplete = false;
bool bleConnected = false;

BLEServer* pServer = nullptr;

BLECharacteristic* dataCharacteristic = nullptr;
BLECharacteristic* commandCharacteristic = nullptr;
BLECharacteristic* statusCharacteristic = nullptr;

uint64_t startTime_us = 0;

float orientation[NUM_IMUS][3] = {
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f}
};

float relativeOrientation[3] = {
    0.0f,
    0.0f,
    0.0f
};

float initialRelativeOrientation[3] = {
    0.0f,
    0.0f,
    0.0f
};

uint64_t lastTimestamp_us[NUM_IMUS] = {
    0,
    0
};

float gyroBias[NUM_IMUS][3] = {
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f}
};

float accelBias[NUM_IMUS][3] = {
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f}
};

bool servoConnected[2] = {
    false,
    false
};

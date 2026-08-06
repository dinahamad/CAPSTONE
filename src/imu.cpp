// ============================================================
// Wearable Stabilization Device for Tremors
//
// File: imu.cpp
//
// Description:
// Initializes and reads the two ICM-20948 IMUs.
//
// Authors:
// - Didi Dimitrova
// ============================================================

#include "imu.h"

// ============================================================
// GLOBAL OBJECTS
// ============================================================

ICM_20948_SPI imu[NUM_IMUS];
IMUData imuData[NUM_IMUS];

// ============================================================
// INITIALIZE IMUs
// ============================================================

bool initializeIMUs()
{
    // Initialize SPI bus
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN);

    bool success = true;

    // ========================================================
    // Initialize Hand IMU
    // ========================================================

    imu[HAND_IMU].begin(CS_HAND, SPI);

    if (imu[HAND_IMU].status != ICM_20948_Stat_Ok)
    {
        Serial.println("ERROR: Hand IMU not detected.");
        success = false;
    }
    else
    {
        Serial.println("Hand IMU connected.");
    }

    // ========================================================
    // Initialize Forearm IMU
    // ========================================================

    imu[FOREARM_IMU].begin(CS_FOREARM, SPI);

    if (imu[FOREARM_IMU].status != ICM_20948_Stat_Ok)
    {
        Serial.println("ERROR: Forearm IMU not detected.");
        success = false;
    }
    else
    {
        Serial.println("Forearm IMU connected.");
    }

    return success;
}

// ============================================================
// READ IMU
// ============================================================

bool readIMU(uint8_t index)
{
    // Read new sensor data
    imu[index].getAGMT();

    if (imu[index].status != ICM_20948_Stat_Ok)
    {
        return false;
    }

    // Accelerometer (g)
    imuData[index].ax = imu[index].accX();
    imuData[index].ay = imu[index].accY();
    imuData[index].az = imu[index].accZ();

    // Gyroscope (deg/s)
    imuData[index].gx = imu[index].gyrX();
    imuData[index].gy = imu[index].gyrY();
    imuData[index].gz = imu[index].gyrZ();

    return true;
}
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

// Creates one ICM-20948 object for each IMU
ICM_20948_SPI imu[NUM_IMUS];

// Stores the latest accelerometer, gyroscope, and orientation data for each IMU
IMUData imuData[NUM_IMUS];

bool handIMUConnected = false;
bool forearmIMUConnected = false;

// ============================================================
// INITIALIZE IMUs
// ============================================================

// Initializes the SPI bus and connects both the hand and forearm IMUs.
bool initializeIMUs()
{
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN);
    bool success = true;

    // Initialize Hand IMU
    imu[HAND_IMU].begin(CS_HAND, SPI);

    handIMUConnected =
        (imu[HAND_IMU].status == ICM_20948_Stat_Ok);

    if (!handIMUConnected)
    {
        Serial.println("ERROR: Hand IMU not detected.");
        success = false;
    }
    else
    {
        Serial.println("Hand IMU connected.");
    }

    // Initialize Forearm IMU
    imu[FOREARM_IMU].begin(CS_FOREARM, SPI);

    forearmIMUConnected =
        (imu[FOREARM_IMU].status == ICM_20948_Stat_Ok);

    if (!forearmIMUConnected)
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

// Reads the latest accelerometer and gyroscope data from the selected IMU.
bool readIMU(uint8_t index)
{
    // Read new sensor data
    imu[index].getAGMT();

    if (imu[index].status != ICM_20948_Stat_Ok)
    {
        if (index == HAND_IMU)
            handIMUConnected = false;
        else if (index == FOREARM_IMU)
            forearmIMUConnected = false;

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

    if (index == HAND_IMU)
        handIMUConnected = true;
    else if (index == FOREARM_IMU)
        forearmIMUConnected = true;
    
    return true;
}

// ============================================================
// IMU CONNECTION STATUS
// ============================================================

void updateIMUConnectionStatus()
{
    uint8_t handID = imu[HAND_IMU].getWhoAmI();
    uint8_t forearmID = imu[FOREARM_IMU].getWhoAmI();

    handIMUConnected = (handID == 0xEA);
    forearmIMUConnected = (forearmID == 0xEA);
}


bool isHandIMUConnected()
{
    return handIMUConnected;
}

bool isForearmIMUConnected()
{
    return forearmIMUConnected;
}
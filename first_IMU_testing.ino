#include <Arduino.h>
#include <SPI.h>
#include "ICM_20948.h"
#include <math.h>

// IMU Setup ============================================================

// SPI Pins:
#define SCK_PIN  7
#define MISO_PIN 8
#define MOSI_PIN 9
#define CS_HAND 3

// Create imu object
ICM_20948_SPI imu;

void setup()
{
    // Start the Serial Monitor
    Serial.begin(115200);
    // Give computer time to connect
    delay(2000);
    Serial.println("Starting IMU...");

    // SPI connection
    SPI.begin(
        SCK_PIN,
        MISO_PIN,
        MOSI_PIN
    );

    pinMode(
        CS_HAND,
        OUTPUT
    );

    digitalWrite(
        CS_HAND,
        HIGH
    );

    imu.begin(
        CS_HAND,
        SPI
    );
}


// Main loop ============================================================

void loop()
{
    // Check whether the IMU has a new measurement
    if (imu.dataReady())
    {
        // Read the newest IMU data
        imu.getAGMT();

        // Read accelerometer values
        float ax = imu.accX();
        float ay = imu.accY();
        float az = imu.accZ();

        // Calculate angle relative to ground (pitch angle)
        // when hand parallel to ground: pitch ~ 0deg
        // pitch can be - or +

        // use trig, convert to rad with *(180/Pi)
        float angleRelativeToGround = atan2(-ax, az) * (180.0 / PI); //rad

        Serial.print("Angle relative to ground: ");
        Serial.print(angleRelativeToGround,2); // 2 decimal places
        Serial.println(" degrees");
    }

    // Wait 100 milliseconds, 10 readings per second
    delay(100);
}
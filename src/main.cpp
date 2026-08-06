// // ============================================================
// // Wearable Stabilization Device for Tremors
// //
// // File: main.cpp
// //
// // Description:
// // Main program.
// // Initializes the IMUs and continuously calculates the
// // wrist pitch.
// //
// // Authors:
// // - Didi Dimitrova
// // ============================================================

// #include <Arduino.h>

// #include "imu.h"
// #include "orientation.h"
// #include "calibration.h"
// #include "tremor.h"

// // ============================================================
// // SETUP
// // ============================================================

// void setup()
// {
//     Serial.begin(115200);

//     while (!Serial)
//     {
//         delay(10);
//     }

//     Serial.println();
//     Serial.println("======================================");
//     Serial.println(" Tremor Detection Prototype");
//     Serial.println("======================================");

//     // Initialize IMUs
//     if (!initializeIMUs())
//     {
//         Serial.println("ERROR: IMU initialization failed.");

//         while (true)
//         {
//             delay(1000);
//         }
//     }

//     // --------------------------------------------------------
//     // Calibration
//     // --------------------------------------------------------

//     Serial.println();
//     Serial.println("SYSTEM READY");
//     Serial.println();
//     Serial.println("Place your wrist in a comfortable neutral position.");
//     Serial.println("Type CAL and press Enter to calibrate.");

//     while (true)
//     {
//         if (Serial.available())
//         {
//             String command = Serial.readStringUntil('\n');
//             command.trim();
//             command.toUpperCase();

//             if (command == "CAL")
//             {
//                 calibrate();

//                 Serial.println();
//                 Serial.println("Calibration Complete.");
//                 Serial.println();
//                 break;
//             }
//             else
//             {
//                 Serial.println("Unknown command.");
//                 Serial.println("Type CAL to calibrate.");
//             }
//         }

//         delay(10);
//     }

//     Serial.println("Starting wrist tracking...");
//     Serial.println();
// }

// // ============================================================
// // LOOP
// // ============================================================

// void loop()
// {
//     if (!updateOrientation(HAND_IMU))
//     {
//         Serial.println("Hand IMU read failed.");
//         return;
//     }

//     if (!updateOrientation(FOREARM_IMU))
//     {
//         Serial.println("Forearm IMU read failed.");
//         return;
//     }

//     float relativePitch = getRelativePitch();
//     float neutralPitch  = getNeutralPitch();
//     float wristPitch    = getCalibratedPitch();

//     Serial.print("Hand: ");
//     Serial.print(imuData[HAND_IMU].pitch, 2);

//     Serial.print(" | Forearm: ");
//     Serial.print(imuData[FOREARM_IMU].pitch, 2);

//     Serial.print(" | Relative: ");
//     Serial.print(relativePitch, 2);

//     Serial.print(" | Neutral: ");
//     Serial.print(neutralPitch, 2);

//     Serial.print(" | Calibrated: ");
//     Serial.println(wristPitch, 2);

//     delay(10);
// }


// ============================================================
// Wearable Stabilization Device for Tremors
//
// File: main.cpp
//
// Description:
// Main application.
// Reads the IMUs, computes the calibrated wrist pitch,
// and feeds it into the tremor detector.
// ============================================================

#include <Arduino.h>

#include "imu.h"
#include "orientation.h"
#include "calibration.h"
#include "tremor_detection.h"

// ============================================================
// SETUP
// ============================================================

void setup()
{
    Serial.begin(115200);

    while (!Serial)
    {
        delay(10);
    }

    Serial.println();
    Serial.println("======================================");
    Serial.println(" Tremor Detection Prototype");
    Serial.println("======================================");

    if (!initializeIMUs())
    {
        Serial.println("ERROR: IMU initialization failed.");

        while (true)
        {
            delay(1000);
        }
    }

    Serial.println();
    Serial.println("SYSTEM READY");
    Serial.println("Place wrist in neutral position.");
    Serial.println("Type CAL then press ENTER.");

    while (true)
    {
        if (Serial.available())
        {
            String command = Serial.readStringUntil('\n');
            command.trim();
            command.toUpperCase();

            if (command == "CAL")
            {
                calibrate();
                break;
            }

            Serial.println("Type CAL to calibrate.");
        }

        delay(10);
    }

    initializeTremorDetection();

    Serial.println();
    Serial.println("Tremor detector started.");
}

// ============================================================
// LOOP
// ============================================================

void loop()
{
    if (!updateOrientation(HAND_IMU))
        return;

    if (!updateOrientation(FOREARM_IMU))
        return;

    float wristPitch = getCalibratedPitch();

    updateTremorDetection(
        imuData[HAND_IMU].pitch,
        imuData[FOREARM_IMU].pitch,
        wristPitch);

    static uint32_t lastPrint = 0;

    if (millis() - lastPrint >= 100)
    {
        printTremorData();
        lastPrint = millis();
    }

    delay(10);
}
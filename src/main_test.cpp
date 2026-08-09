// // ============================================================
// // Wearable Stabilization Device for Tremors
// //
// // File: main.cpp
// //
// // Description:
// // Main application.
// // Reads the IMUs, computes the calibrated wrist pitch,
// // and feeds it into the tremor detector.
// // ============================================================

// #include <Arduino.h>

// #include "imu.h"
// #include "orientation.h"
// #include "calibration.h"
// #include "tremor_detection.h"

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

//     if (!initializeIMUs())
//     {
//         Serial.println("ERROR: IMU initialization failed.");

//         while (true)
//         {
//             delay(1000);
//         }
//     }

//     Serial.println();
//     Serial.println("SYSTEM READY");
//     Serial.println("Place wrist in neutral position.");
//     Serial.println("Type CAL then press ENTER.");

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
//                 break;
//             }

//             Serial.println("Type CAL to calibrate.");
//         }

//         delay(10);
//     }

//     initializeTremorDetection();

//     Serial.println();
//     Serial.println("Tremor detector started.");
// }

// // ============================================================
// // LOOP
// // ============================================================

// void loop()
// {
//     if (!updateOrientation(HAND_IMU))
//         return;

//     if (!updateOrientation(FOREARM_IMU))
//         return;

//     float wristPitch = getCalibratedPitch();

//     updateTremorDetection(
//         imuData[HAND_IMU].pitch,
//         imuData[FOREARM_IMU].pitch,
//         wristPitch);

//     static uint32_t lastPrint = 0;

//     if (millis() - lastPrint >= 100)
//     {
//         printTremorData();
//         lastPrint = millis();
//     }

//     delay(10);
// }
#include <Arduino.h>
#include <esp_sleep.h>
#include "Battery.h"
#include "State.h"
#include "UI.h"
#include "UARTLink.h"
#include "pins.h"
#include "imu.h"
#include "tremor_detection.h"
#include "orientation.h"
#include "calibration.h"
#include "servo.h"
#include "calibrate_state.h"

void setup()
{

    Serial.begin(115200);
    UARTLink_init();
    CALIBRATION_STATE = false;


    systemState = LIGHT_SLEEP;
    stableState = SENSE;


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

    initializeTremorDetection();
    setupServos();

    Serial.println();
    Serial.println("Tremor detector started.");

}


// void loop()
// {

//     UARTLink_receive();
//     UARTLink_update();

//     Serial.print("System = ");
//     Serial.print(systemState);

//     Serial.print("  Stable = ");
//     Serial.println(stableState);

//     UARTLink_receive();
//     UARTLink_update();
    
//     if (stableState == SENSE)
//     {

//         if (!updateOrientation(HAND_IMU))
//             return;

//         if (!updateOrientation(FOREARM_IMU))
//             return;

//         float wristPitch = getCalibratedPitch();

//         updateTremorDetection(
//             imuData[HAND_IMU].pitch,
//             imuData[FOREARM_IMU].pitch,
//             wristPitch);

//         static uint32_t lastPrint = 0;

//         if (millis() - lastPrint >= 100)
//         {
//             printTremorData();
//             lastPrint = millis();
//         }

//         delay(10);
        
//     }
//     else if (stableState == STABILIZE)
//     {
//         // run stabilization algorithm
//     }


// }


void loop()
{
    UARTLink_receive();
    UARTLink_update();

    // -----------------------------
    // If asleep, do nothing
    // -----------------------------
    if (systemState == LIGHT_SLEEP)
    {
        delay(10);
        return;
    }

    // -----------------------------
    // Awake
    // -----------------------------
    if (stableState == SENSE)
    {
        // if (CALIBRATION_STATE){
        //     originAngles();
        //     delay(100);
        //     CALIBRATION_STATE = false;
        // }
        servosSlack();

        if (!updateSensing())
        {
            return;
        }

        float wristPitch = getCalibratedPitch();

        updateTremorDetection(imuData[HAND_IMU].pitch,imuData[FOREARM_IMU].pitch,wristPitch);

        static uint32_t lastPrint = 0;

        if (millis() - lastPrint >= 100)
            {
                printTremorData();                  // dina added
                // Serial.print(" | Top servo: ");
                // Serial.print(getTopServoAngle());
                // Serial.print(" deg");

                // Serial.print(" | Bottom servo: ");
                // Serial.print(getBottomServoAngle());
                // Serial.println(" deg");
                lastPrint = millis();
            }

    }
    else if (stableState == STABILIZE)
    {
        if (!updateSensing())
        {
            return;
        }

        float wristPitch = getCalibratedPitch();

        updateTremorDetection(imuData[HAND_IMU].pitch,imuData[FOREARM_IMU].pitch,wristPitch);

        updateServos(wristPitch);

        static uint32_t lastPrint = 0;

        if (millis() - lastPrint >= 100)
            {
                printTremorData();                  // dina added
                // Serial.print(" | Top servo: ");
                // Serial.print(getTopServoAngle());
                // Serial.print(" deg");

                // Serial.print(" | Bottom servo: ");
                // Serial.print(getBottomServoAngle());
                // Serial.println(" deg");
                lastPrint = millis();
            }
    }

    delay(5);
}
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
#include "ble_manager.h"

void setup()
{

    Serial.begin(115200);
    BLEManager_init();
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

    // ========================================================
    // HARDWARE STATUS
    // ========================================================

    static uint32_t lastHardwareStatus = 0;

    if (millis() - lastHardwareStatus >= 500)
    {
        updateIMUConnectionStatus();

        String packet = "HARDWARE," +
            String(isHandIMUConnected() ? 1 : 0) + "," +
            String(isForearmIMUConnected() ? 1 : 0) + "," +
            String(isTopServoConnected() ? 1 : 0) + "," +
            String(isBottomServoConnected() ? 1 : 0);

        Serial.println(packet);
        BLEManager_send(packet);

        lastHardwareStatus = millis();
    }

    // ========================================================
    // POWER / MODE STATUS
    // ========================================================

    static uint32_t lastSystemStatus = 0;

    if (millis() - lastSystemStatus >= 500)
    {
        if (systemState == AWAKE)
        {
            BLEManager_send("POWER,ON");

            if (stableState == SENSE)
            {
                BLEManager_send("MODE,SENSE");
            }
            else
            {
                BLEManager_send("MODE,STABILIZE");
            }
        }
        else
        {
            BLEManager_send("POWER,OFF");
        }

        lastSystemStatus = millis();
    }

    // ========================================================
    // DEVICE OFF
    // ========================================================

    if (systemState == LIGHT_SLEEP)
    {
        delay(10);
        return;
    }

    // ========================================================
    // SENSING MODE
    // ========================================================

    if (stableState == SENSE)
    {
        servosSlack();

        if (!updateSensing())
        {
            return;
        }

        float wristPitch = getCalibratedPitch();

        static uint32_t lastPrint = 0;

        if (millis() - lastPrint >= 100)
        {
            printTremorData();
            lastPrint = millis();
        }
    }

    // ========================================================
    // STABILIZATION MODE
    // ========================================================

    else if (stableState == STABILIZE)
    {
        if (!updateSensing())
        {
            return;
        }

        float wristPitch = getCalibratedPitch();

        updateServos(wristPitch);

        static uint32_t lastPrint = 0;

        if (millis() - lastPrint >= 100)
        {
            printTremorData();
            lastPrint = millis();
        }
    }

    delay(10);
}
#include <Arduino.h>
#include <esp_sleep.h>
#include "Battery.h"
#include "State.h"
#include "UI.h"
#include "UARTLink.h"
#include "Pins.h"

void setup()
{

    Serial.begin(115200);
    UARTLink_init();


    systemState = AWAKE;
    stableState = SENSE;

}


void loop()
{

    UARTLink_receive();
    UARTLink_update();
    
    if (stableState == SENSE)
    {
        // IMU sensing only
    }
    else if (stableState == STABILIZE)
    {
        // run stabilization algorithm
    }


}
#include <Arduino.h>
#include "pins.h"
#include "UARTLink.h"
#include "State.h"
#include "UI.h"
#include "Battery.h"
#include "driver/uart.h"
#include "Wake.h"
#include "servo.h"
#include "calibrate_state.h"

static volatile bool ackReceived = false;

#ifdef USE_UART_LINK
HardwareSerial Link(1);
#endif

void UARTLink_init()
{
#ifdef USE_UART_LINK
    Link.begin(115200, SERIAL_8N1, LINK_RX_PIN, LINK_TX_PIN);
    Serial.println("UART initialized");
#endif
}

void shutdownBeforeSleep()
{

    stableState = SENSE;
    Serial.println("Shutdown before sleep");
    servosSlack();
    // Disable motors
    // Disable sensors
    // move to senses
    // Save data if needed

}

void UARTLink_receive()
{
#ifdef USE_UART_LINK

    while (Link.available())
    {
        String command = Link.readStringUntil('\n');
        command.trim();

        Serial.print("UART RX: ");
        Serial.println(command);

        if (command == "ACK")
        {
            ackReceived = true;
        }
        else if (command == "SLEEP")
        {
            Link.println("ACK");

            systemState = LIGHT_SLEEP;

            goToLightSleep();
        }
        else if (command == "AWAKE")
        {
            Link.println("ACK");

            systemState = AWAKE;
        }
        else if (command == "SENSE")
        {
            Link.println("ACK");

            stableState = SENSE;
        }
        else if (command == "STABILIZE")
        {
            Link.println("ACK");

            stableState = STABILIZE;
        }
        else if (command == "CALIBRATE")
        {
            Link.println("ACK");

            CALIBRATION_STATE = true;
        
            // Run calibration here
            // calibrateSensors();
        }

    }

#endif
}

void goToLightSleep()
{
    shutdownBeforeSleep();

    esp_sleep_enable_ext0_wakeup(
        (gpio_num_t)WAKE_IN_PIN,
        HIGH
    );

    esp_light_sleep_start();

}

void UARTLink_update()
{
    static unsigned long lastBattery = 0;

    if (millis() - lastBattery >= 1000)
    {
        lastBattery = millis();

        float battery = Battery_getPercentage();

        Link.print("BAT:");
        Link.println((int)battery);
    }
}
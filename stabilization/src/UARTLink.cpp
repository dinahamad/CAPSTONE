#include <Arduino.h>
#include "Pins.h"
#include "UARTLink.h"
#include "State.h"
#include "UI.h"
#include "Battery.h"
#include "driver/uart.h"
#include "Wake.h"

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

void shutdownBeforeSleep() // servo slacks
{
    // Disable motors
    // Disable sensors
    // Save data if needed

    Serial.println("Shutdown before sleep");
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
    }

#endif
}

void goToLightSleep()
{
    shutdownBeforeSleep();

    Serial.print("Wake pin before sleep: ");
    Serial.println(digitalRead(WAKE_IN_PIN));

    esp_sleep_enable_ext0_wakeup(
        (gpio_num_t)WAKE_IN_PIN,
        HIGH
    );

    Serial.println("Entering light sleep");

    esp_light_sleep_start();

    Serial.println("Woke from light sleep");
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

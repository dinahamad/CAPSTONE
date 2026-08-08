#include <Arduino.h>
#include "Pins.h"
#include "UARTLink.h"
#include "State.h"

static volatile bool ackReceived = false;
static float slaveBattery = 100.0;

#ifdef USE_UART_LINK
HardwareSerial Link(1);
#endif

void UARTLink_init()
{
#ifdef USE_UART_LINK
    Link.begin(115200, SERIAL_8N1, LINK_RX_PIN, LINK_TX_PIN);
#endif
}

bool UARTLink_sendState(SystemState state)
{
#ifdef USE_UART_LINK

    for (int attempt = 0; attempt < 3; attempt++)
    {
        if (state == LIGHT_SLEEP)
            Link.println("SLEEP");
        else
            Link.println("AWAKE");

        unsigned long start = millis();

        while (millis() - start < 500)
        {
                UARTLink_receive();
                if (ackReceived)
                    return true;
        }

        Serial.println("Retrying...");
    }

    Serial.println("No ACK received.");
    return false;

#else

    return true;

#endif
}

bool UARTLink_sendState(StableState state)
{
#ifdef USE_UART_LINK

    for (int attempt = 0; attempt < 3; attempt++)
    {
        if (state == SENSE)
            Link.println("SENSE");
        else
            Link.println("STABILIZE");

        unsigned long start = millis();

        while (millis() - start < 500)
        {
            if (Link.available())
            {
                UARTLink_receive();
                if (ackReceived)
                    return true;
            }
        }

        Serial.println("Retrying...");
    }

    Serial.println("No ACK received.");
    return false;

#else

    return true;

#endif
}

void UARTLink_sendCalibrate()
{
#ifdef USE_UART_LINK
    Link.println("CALIBRATE");
#endif
}

void UARTLink_receive()
{
#ifdef USE_UART_LINK

    while (Link.available())
    {
        String command = Link.readStringUntil('\n');
        command.trim();

        // Serial.print("UART RX: ");
        // Serial.println(command);

        if (command == "ACK")
        {
            ackReceived = true;
        }
        else if (command.startsWith("BAT:"))
        {
            slaveBattery = command.substring(4).toFloat();
        }
    }

#endif
}

float UARTLink_getSlaveBattery()
{
    return slaveBattery;
}

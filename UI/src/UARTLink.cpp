#include <Arduino.h>
#include "Pins.h"
#include "UARTLink.h"
#include "State.h"

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
            if (Link.available())
            {
                String reply = Link.readStringUntil('\n');
                reply.trim();

                if (reply == "ACK")
                {
                    Serial.println("ACK received.");
                    return true;
                }
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

bool UARTLink_sendState(StableState state)
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
            if (Link.available())
            {
                String reply = Link.readStringUntil('\n');
                reply.trim();

                if (reply == "ACK")
                {
                    Serial.println("ACK received.");
                    return true;
                }
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

// For recieving
// if (Link.available())
// {
//     String msg = Link.readStringUntil('\n');
//     msg.trim();

//     if (msg == "SLEEP")
//     {
//         // enter sleep mode
//     }
//     else if (msg == "AWAKE")
//     {
//         // wake mode
//     }

//     Link.println("ACK");
// }

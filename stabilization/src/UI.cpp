#include <Arduino.h>
#include <esp_sleep.h>
#include "Battery.h"
#include "State.h"
#include "UARTLink.h"
#include "Pins.h"

void USB_detect_init() {
    pinMode(USB_VBUS_PIN, INPUT);
}

bool USB_connected()
{
    return analogRead(USB_VBUS_PIN) > 2000;
}
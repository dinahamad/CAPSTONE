#include <Arduino.h>
#include <esp_sleep.h>
#include "Wake.h"
#include "Pins.h"


void Wake_init()
{
    pinMode(WAKE_IN_PIN, INPUT);
}


void Wake_enable()
{
    Serial.print("Wake pin state before sleep: ");
    Serial.println(digitalRead(WAKE_IN_PIN));

    esp_err_t err = esp_sleep_enable_ext0_wakeup(
        (gpio_num_t)WAKE_IN_PIN,
        1   // wake when HIGH
    );

    Serial.print("Wake enable result: ");
    Serial.println(err);
}
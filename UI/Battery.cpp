#include <Arduino.h>
#include "Battery.h"

// On the Feather V2, the battery divider is hardwired to A13
#define VBAT_PIN A13


void Battery_init() {
    pinMode(VBAT_PIN, INPUT);
}


float Battery_getVoltage() {

    float vbat_mv = analogReadMilliVolts(VBAT_PIN);

    // Feather V2 has a 200k/200k divider
    return (vbat_mv * 2.0) / 1000.0;
}


float Battery_getPercentage() {

    float voltage = Battery_getVoltage();

    float percentage = ((voltage - 3.2) / (4.2 - 3.2)) * 100.0;

    if (percentage > 100.0) percentage = 100.0;
    if (percentage < 0.0) percentage = 0.0;

    return percentage;
}


void Battery_printStatus() {

    Serial.print("Battery Voltage: ");
    Serial.print(Battery_getVoltage(), 2);
    Serial.print(" V | ");

    Serial.print("Approx. Charge: ");
    Serial.print(Battery_getPercentage(), 1);
    Serial.println("%");
}

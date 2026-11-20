#include <Arduino.h>
#include "Battery.h"
#include "Pins.h"

#define BATTERY_DIVIDER_RATIO 4.0f //measure this !!!!!!!!!

// 2S LiPo limits
#define BATTERY_MAX_VOLTAGE 8.4f
#define BATTERY_MIN_VOLTAGE 6.6f  // 3.3V per cell: shutdown threshold

bool batteryLow_check = false;


void Battery_init() {
    pinMode(VBAT_PIN, INPUT);
    analogSetPinAttenuation(VBAT_PIN, ADC_11db);
}


// Read actual battery voltage (2S LiPo)
float Battery_getVoltage() {

    // Average multiple ADC readings for stability
    float adc_mv = 0;

    for (int i = 0; i < 10; i++) {
        adc_mv += analogReadMilliVolts(VBAT_PIN);
        delay(2);
    }

    adc_mv /= 10.0f;

    // 300k / 100k voltage divider = 4:1
    return (adc_mv * BATTERY_DIVIDER_RATIO) / 1000.0f;
}


// Calculate battery percentage for 2S LiPo
float Battery_getPercentage() {

    float voltage = Battery_getVoltage();

    float percentage =
        ((voltage - BATTERY_MIN_VOLTAGE) /
        (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE)) * 100.0f;

    if (percentage > 100.0f) percentage = 100.0f;
    if (percentage < 0.0f) percentage = 0.0f;

    return percentage;
}


// Low battery protection (latched)
bool Check_Battery_isLow() {

    if (Battery_getVoltage() <= BATTERY_MIN_VOLTAGE) {
        batteryLow_check = true;
    }
    else{
        batteryLow_check = false;
    }
    return batteryLow_check;
}


// Battery status output
void Battery_printStatus() {

    float voltage = Battery_getVoltage();
    float percentage = Battery_getPercentage();

    Serial.print("Battery Voltage: ");
    Serial.print(voltage, 2);
    Serial.print(" V | ");

    Serial.print("Approx. Charge: ");
    Serial.print(percentage, 1);
    Serial.print("% | ");
}
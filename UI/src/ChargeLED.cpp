#include <Arduino.h>
#include "ChargeLED.h"
#include "Pins.h"


unsigned long lastBlink = 0;
bool blinkState = false;


void ChargeLED_init() {

    pinMode(CHARGE_LED1, OUTPUT);
    pinMode(CHARGE_LED2, OUTPUT);
    pinMode(CHARGE_LED3, OUTPUT);

    digitalWrite(CHARGE_LED1, LOW);
    digitalWrite(CHARGE_LED2, LOW);
    digitalWrite(CHARGE_LED3, LOW);
}


void setLEDs(bool led1, bool led2, bool led3) {

    digitalWrite(CHARGE_LED1, led1);
    digitalWrite(CHARGE_LED2, led2);
    digitalWrite(CHARGE_LED3, led3);
}

void LEDsoff() {
    digitalWrite(CHARGE_LED1, LOW);
    digitalWrite(CHARGE_LED2, LOW);
    digitalWrite(CHARGE_LED3, LOW);
}


void ChargeLED_update(float batteryPercent, bool usbConnected) {

    // Blink every 500 ms
    if (millis() - lastBlink > 500) {
        lastBlink = millis();
        blinkState = !blinkState;
    }


    // ===========================
    // NOT CHARGING (battery mode)
    // ===========================
    if (!usbConnected) {

        if (batteryPercent < 20) {
            // LED1 flashing
            setLEDs(blinkState, false, false);
        }

        else if (batteryPercent < 40) {
            // LED1 solid
            setLEDs(true, false, false);
        }

        else if (batteryPercent < 70) {
            // LED1 + LED2
            setLEDs(true, true, false);
        }

        else {
            // LED1 + LED2 + LED3
            setLEDs(true, true, true);
        }
    }


    // ===========================
    // CHARGING (USB connected)
    // ===========================
    else {

        if (batteryPercent < 33) {
            // LED1 flashing
            setLEDs(blinkState, false, false);
        }

        else if (batteryPercent < 66) {
            // LED1 solid, LED2 flashing
            setLEDs(true, blinkState, false);
        }

        else if (batteryPercent < 100) {
            // LED1 + LED2 solid, LED3 flashing
            setLEDs(true, true, blinkState);
        }

        else {
            // Full
            setLEDs(true, true, true);
        }
    }
}
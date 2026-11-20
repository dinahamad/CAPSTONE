#include <Arduino.h>
#include "ChargeLED.h"
#include "Pins.h"

static unsigned long lastBlink2 = 0;
static bool blinkState2 = false;


void ChargeLED_init() {

    pinMode(CHARGE_LED1, OUTPUT);
    pinMode(CHARGE_LED2, OUTPUT);
    pinMode(CHARGE_LED3, OUTPUT);

    analogWrite(CHARGE_LED1, 0);
    analogWrite(CHARGE_LED2, 0);
    analogWrite(CHARGE_LED3, 0);
}


void setLEDs(bool led1, bool led2, bool led3) {

    analogWrite(CHARGE_LED1, led1 ? LED_BRIGHTNESS : 0);
    analogWrite(CHARGE_LED2, led2 ? LED_BRIGHTNESS : 0);
    analogWrite(CHARGE_LED3, led3 ? LED_BRIGHTNESS : 0);
}


void LEDsoff() {

    analogWrite(CHARGE_LED1, 0);
    analogWrite(CHARGE_LED2, 0);
    analogWrite(CHARGE_LED3, 0);
}

void LowPower_flash() {

    if (millis() - lastBlink2 > 500) {
        lastBlink2= millis();
        blinkState2 = !blinkState2;
    }

    setLEDs(blinkState2, false, false);
}


void ChargeLED_update(float batteryPercent, bool usbConnected) {

    if (millis() - lastBlink2 > 500) {
        lastBlink2 = millis();
        blinkState2 = !blinkState2;
    }


    if (!usbConnected) {

        if (batteryPercent < 20) {
            setLEDs(blinkState2, false, false);
        }

        else if (batteryPercent < 40) {
            setLEDs(true, false, false);
        }

        else if (batteryPercent < 70) {
            setLEDs(true, true, false);
        }

        else {
            setLEDs(true, true, true);
        }
    }


    else {

        if (batteryPercent < 33) {
            setLEDs(blinkState2, false, false);
        }

        else if (batteryPercent < 66) {
            setLEDs(true, blinkState2, false);
        }

        else if (batteryPercent < 100) {
            setLEDs(true, true, blinkState2);
        }

        else {
            setLEDs(true, true, true);
        }
    }
}


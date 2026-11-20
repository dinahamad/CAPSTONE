#include <Arduino.h>
#include "PowerLED.h"
#include "Pins.h"

static unsigned long lastBlink1 = 0;
static bool blinkState1 = false;

void LED_init() {

  pinMode(LED_RED, OUTPUT);
  ledcSetup(GREEN_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(LED_GREEN, GREEN_CH);
  ledcSetup(BLUE_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(LED_BLUE, BLUE_CH);

  LED_off();
}


void LED_sleep() {
  // Orange
  digitalWrite(LED_RED, HIGH);
  ledcWrite(GREEN_CH, 0);
  ledcWrite(BLUE_CH, 0);
}


void LED_awake() {
  // Dim green
  digitalWrite(LED_RED, LOW);
  ledcWrite(GREEN_CH, LED_BRIGHTNESS*2);
  ledcWrite(BLUE_CH, 0);
}


void LED_stable() {
  // Blue
  digitalWrite(LED_RED, LOW);
  ledcWrite(GREEN_CH, 0);
  ledcWrite(BLUE_CH, LED_BRIGHTNESS*2);
}


void LED_off() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(GREEN_CH, LOW);
  digitalWrite(LED_BLUE, LOW);
}

void LED_lowpower() {
    if (millis() - lastBlink1 >= 500) {
        lastBlink1 = millis();
        blinkState1 = !blinkState1;
    }

    digitalWrite(LED_RED, blinkState1 ? HIGH : LOW);
    ledcWrite(GREEN_CH, 0);
    ledcWrite(BLUE_CH, 0);
}
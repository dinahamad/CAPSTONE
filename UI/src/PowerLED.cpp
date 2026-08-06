#include <Arduino.h>
#include "PowerLED.h"
#include "Pins.h"

void LED_init() {

  ledcSetup(RED_CH, PWM_FREQ, PWM_RES);
  ledcSetup(GREEN_CH, PWM_FREQ, PWM_RES);
  ledcSetup(BLUE_CH, PWM_FREQ, PWM_RES);

  ledcAttachPin(LED_RED, RED_CH);
  ledcAttachPin(LED_GREEN, GREEN_CH);
  ledcAttachPin(LED_BLUE, BLUE_CH);

  LED_off();
}


void LED_sleep() {
  // Orange
  ledcWrite(RED_CH, LED_BRIGHTNESS);
  ledcWrite(GREEN_CH, 0);
  ledcWrite(BLUE_CH, 0);
}


void LED_awake() {
  // Blue
  ledcWrite(RED_CH, 0);
  ledcWrite(GREEN_CH, LED_BRIGHTNESS);
  ledcWrite(BLUE_CH, 0);
}

void LED_charging() {
  // Blue
  ledcWrite(RED_CH, 0);
  ledcWrite(GREEN_CH, 0);
  ledcWrite(BLUE_CH, LED_BRIGHTNESS);
}


void LED_off() {
  ledcWrite(RED_CH, 0);
  ledcWrite(GREEN_CH, 0);
  ledcWrite(BLUE_CH, 0);
}
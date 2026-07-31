#include <Arduino.h>
#include "PowerLED.h"

#define LED_RED   15
#define LED_GREEN 32
#define LED_BLUE  14

#define RED_CH    0
#define GREEN_CH  1
#define BLUE_CH   2

#define PWM_FREQ  5000
#define PWM_RES   8


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
  ledcWrite(RED_CH, 255);
  ledcWrite(GREEN_CH, 0);
  ledcWrite(BLUE_CH, 0);
}


void LED_awake() {
  // Blue
  ledcWrite(RED_CH, 0);
  ledcWrite(GREEN_CH, 255);
  ledcWrite(BLUE_CH, 0);
}


void LED_off() {
  ledcWrite(RED_CH, 0);
  ledcWrite(GREEN_CH, 0);
  ledcWrite(BLUE_CH, 0);
}

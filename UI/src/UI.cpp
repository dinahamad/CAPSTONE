#include <Arduino.h>
#include <esp_sleep.h>
#include "PowerLED.h"
#include "Battery.h"
#include "ChargeLED.h"
#include "State.h"
#include "UARTLink.h"
#include "Pins.h"

const unsigned long pressTime = 100;      // must hold >100ms

void Button_init() {
    pinMode(BUTTON_PIN, INPUT);
}

void USB_detect_init() {
    pinMode(USB_VBUS_PIN, INPUT);
}

bool USB_connected()
{
    return analogRead(USB_VBUS_PIN) > 2000;
}


void waitForRelease() {
  while (digitalRead(BUTTON_PIN) == HIGH) {
    delay(10);
  }
}


bool validButtonPress(uint8_t BUTTON_NUMBER) {

  // Wait for HIGH transition
  if (digitalRead(BUTTON_NUMBER) == HIGH) {

    unsigned long start = millis();

    // Confirm it stays HIGH
    while (digitalRead(BUTTON_NUMBER) == HIGH) {
      delay(10);
    }

    unsigned long duration = millis() - start;


    // HIGH -> LOW completed
    if (duration >= pressTime) {
      return true;
    }
  }

  return false;
}

void goToLightSleep() {

  waitForRelease();

  Serial.println("Mode = Light Sleep");
  //UARTLink_sendState(systemState);

  LED_off();
  LEDsoff();

  delay(50);

  esp_sleep_enable_ext0_wakeup(
      (gpio_num_t)BUTTON_PIN,
      HIGH
  );

  esp_light_sleep_start();

  Serial.println("Mode = Awake");
  //UARTLink_sendState(systemState);

  systemState = AWAKE;

  waitForRelease();
}

void changeStableState(){

  if (stableState == SENSE) {
    Serial.println("Mode = Stabilizing");
    stableState = STABILIZE;
    //UARTLink_sendState(stableState);
  }
  else if (stableState == STABILIZE) {
    Serial.println("Mode = Only sensing");
    stableState = SENSE;
    //UARTLink_sendState(stableState);
  }

}

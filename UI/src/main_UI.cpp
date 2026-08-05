#include <Arduino.h>
#include <esp_sleep.h>
#include "PowerLED.h"
#include "Battery.h"
#include "ChargeLED.h"
#include "State.h"
#include "UI.h"
#include "UARTLink.h"
#include "Pins.h"

unsigned long lastPress = 0;
const unsigned long lockoutTime = 1000;   // ignore repeat presses

void setup() {

  Serial.begin(115200);

  Button_init();
  LED_init();
  Battery_init();
  ChargeLED_init();
  USB_detect_init();

  UARTLink_init();

  Serial.println("Starting...");
  delay(500);

  goToLightSleep();
}


void loop() {

  float battery = Battery_getPercentage();
  bool usb = USB_connected();

  // Update battery/charging LEDs continuously
  ChargeLED_update(battery, usb);

  // Update main status LED
  if (usb) { 
    LED_charging();   // Blue LED
  }
  else {
    LED_awake();      // Green LED
  }


  if (systemState == NORMAL) {

    if (millis() - lastPress > lockoutTime) {

      if (validButtonPress()) {

        lastPress = millis();

        systemState = LIGHT_SLEEP;

        goToLightSleep();
      }
    }
  }
}

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
  Wake_init();
  Battery_init();
  ChargeLED_init();
  USB_detect_init();

  UARTLink_init();

  Serial.println("Starting...");
  delay(500);

  goToLightSleep();
}


void loop() {

  float UI_battery = Battery_getPercentage();
  bool usb = USB_connected();
  
  Serial.print("UI Charge: ");
  Serial.print(UI_battery, 1);
  Serial.println("%");
  
  UARTLink_receive();
  float Stabil_battery = UARTLink_getSlaveBattery();

  Serial.print("Stabil. Charge: ");
  Serial.print(Stabil_battery, 1);
  Serial.println("%");

  float total_battery = (UI_battery+Stabil_battery)/2;
  Serial.print("Total. Charge: ");
  Serial.print(total_battery, 1);
  Serial.println("%");

  // Update battery/charging LEDs continuously
  ChargeLED_update(total_battery, usb);

  // Update main status LED
  if (usb) { 
    LED_charging();   // Blue LED
  }
  else {
    LED_awake();      // Green LED
  }

  if (systemState == AWAKE) {

    if (millis() - lastPress > lockoutTime) {

      if (validButtonPress(BUTTON_PIN)) {

        lastPress = millis();

        goToLightSleep();
      }

      if (validButtonPress(STABLE_PIN)) {

        lastPress = millis();

        changeStableState();
      }
    }
  }
}
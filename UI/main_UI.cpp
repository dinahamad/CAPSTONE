#include <Arduino.h>
#include <esp_sleep.h>
#include "LED.h"
#include "Battery.h"
#include "ChargeLED.h"

#define BUTTON_PIN 37//32
#define USB_VBUS_PIN 36

enum State {
  LIGHT_SLEEP,
  NORMAL
};

State systemState = LIGHT_SLEEP;

unsigned long lastPress = 0;

const unsigned long pressTime = 100;      // must hold >100ms
const unsigned long lockoutTime = 1000;   // ignore repeat presses

void goToLightSleep();
bool validButtonPress();
void waitForRelease();

void USB_detect_init() {
    pinMode(USB_VBUS_PIN, INPUT);
}

bool USB_connected() {
    //Serial.println(analogRead(USB_VBUS_PIN));
    return analogRead(USB_VBUS_PIN) > 2000;
}

void waitForRelease() {
  while (digitalRead(BUTTON_PIN) == HIGH) {
    delay(10);
  }
}


bool validButtonPress() {

  // Wait for HIGH transition
  if (digitalRead(BUTTON_PIN) == HIGH) {

    unsigned long start = millis();

    // Confirm it stays HIGH
    while (digitalRead(BUTTON_PIN) == HIGH) {
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



void setup() {

  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT);

  LED_init();
  Battery_init();
  ChargeLED_init();
  USB_detect_init();

  Serial.println("Starting...");
  delay(500);

  goToLightSleep();
}


void loop() {
  // TEST BATTERY LEDS
  // ChargeLED_update(10, false);   // <20%, not charging
  // ChargeLED_update(50, false);   // 40-70%, not charging
  // ChargeLED_update(80, false);   // 70-100%, not charging
  // ChargeLED_update(100, false);  // full, not charging
  // Charging tests:
  // ChargeLED_update(20, true);   // LED1 flashing
  // ChargeLED_update(50, true);   // LED1 solid + LED2 flashing
  // ChargeLED_update(80, true);   // LED1+LED2 solid + LED3 flashing
  // ChargeLED_update(100, true);  // all solid


  float battery = Battery_getPercentage();
  bool usb = USB_connected();

  ChargeLED_update(battery, usb);
  // Battery_printStatus();

  if (systemState == NORMAL) {

    if (millis() - lastPress > lockoutTime) {

      if (validButtonPress()) {
        
        //Serial.println("LOW -> HIGH -> LOW detected");

        lastPress = millis();

        systemState = LIGHT_SLEEP;

        goToLightSleep();
      }
    }
  }
}


void goToLightSleep() {

  // Ensure button is LOW before sleeping
  waitForRelease();

  //Serial.println("Entering Light Sleep...");
  Serial.println("Mode = Sleeping");
  LED_sleep();   // RED LED
  
  delay(50);

  esp_sleep_enable_ext0_wakeup(
    (gpio_num_t)BUTTON_PIN,
    HIGH
  );

  esp_light_sleep_start();


  // Wake occurred because LOW -> HIGH happened

  //Serial.println("Wakeup -> NORMAL MODE");
  Serial.println("Mode = Awake");
  LED_awake();   // Green LED

  systemState = NORMAL;


  // Ignore the wake-up press
  // Wait until HIGH -> LOW before accepting another press
  waitForRelease();

  //Serial.println("Ready for next press");
}

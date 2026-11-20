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
const unsigned long calibrationPressTime = 2000;      // must hold >100ms
bool batteryLow = false;

unsigned long lastSend = 0;
bool sendState = false;

void setup() {

  Serial.begin(115200);

  Button_init();
  LED_init();
  Wake_init();
  Battery_init();
  ChargeLED_init();
  // USB_detect_init();

  UARTLink_init();

  Serial.println("Starting...");
  delay(500);

  goToLightSleep();
}


void loop() {

  float total_battery = Battery_getPercentage();
  //bool usb = USB_connected();

  Serial.println(total_battery);
  batteryLow = Check_Battery_isLow();

  if (batteryLow){
    if (millis() - lastSend > 1000) {
                lastSend = millis();
                sendState = !sendState;
                // send to rx/tx to other board and cut off power to servos!!!!!
                UARTLink_sendLowBattery();
            }   
            
        // power led red
        LED_lowpower();

        // flash charge leds
        LowPower_flash();
  }
  else{
      // Update battery/charging LEDs continuously
    ChargeLED_update(total_battery, false);


    // // Update main status LED
    // if (usb) { 
    //   LED_charging();   // Blue LED
    //   LED_awake();      // Green LED
    // }
    // else {
    //   LED_awake();      // Green LED
    // }

    if (stableState == STABILIZE){
      LED_stable();
    }
    else{
      LED_awake();
    }

  }
  
  if (systemState == AWAKE) {

    if (millis() - lastPress > lockoutTime) {

      if (validButtonPress(POWER_PIN)) {

        lastPress = millis();

        goToLightSleep();
      }

      if (unsigned long duration = validButtonPress(STABLE_PIN)) {

        lastPress = millis();

        if (stableState == SENSE && duration >= calibrationPressTime ){
          UARTLink_sendCalibrate();
          Serial.println("Calibrating...");
        }
        else{
          changeStableState();
        }

      }
    }
  }
}
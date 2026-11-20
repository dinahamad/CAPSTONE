#ifndef PINS_H
#define PINS_H

constexpr uint8_t LED_BRIGHTNESS = 10;   // 0-255 (lower = dimmer)

// Main
constexpr uint8_t POWER_PIN = 25;
constexpr uint8_t STABLE_PIN = 4;
//constexpr uint8_t USB_VBUS_PIN = 36; //change (might be removing??)

// Battery
constexpr uint8_t VBAT_PIN = 36;

constexpr uint8_t CHARGE_LED1 = 37;
constexpr uint8_t CHARGE_LED2 = 38;
constexpr uint8_t CHARGE_LED3 = 39;

// Charging LED
constexpr uint8_t LED_RED   = 27;
constexpr uint8_t LED_GREEN = 26;
constexpr uint8_t LED_BLUE  = 14;

// Power LED
constexpr uint8_t RED_CH   = 0;
constexpr uint8_t GREEN_CH = 1;
constexpr uint8_t BLUE_CH  = 2;

constexpr uint16_t PWM_FREQ = 5000;
constexpr uint8_t PWM_RES   = 8;

// UART Link
constexpr uint8_t LINK_TX_PIN = 7; 
constexpr uint8_t LINK_RX_PIN = 8;

// Wake line
constexpr uint8_t WAKE_OUT_PIN = 13;

// Comment this out to disable UART communication
#define USE_UART_LINK

#endif
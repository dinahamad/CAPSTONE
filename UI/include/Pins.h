#ifndef PINS_H
#define PINS_H

constexpr uint8_t LED_BRIGHTNESS = 10;   // 0-255 (lower = dimmer)

// Main
constexpr uint8_t BUTTON_PIN = 34;
constexpr uint8_t STABLE_PIN = 35;
//constexpr uint8_t USB_VBUS_PIN = 36;

// Battery
constexpr uint8_t VBAT_PIN = 4;

constexpr uint8_t CHARGE_LED1 = 5;
constexpr uint8_t CHARGE_LED2 = 6;
constexpr uint8_t CHARGE_LED3 = 7;

// Charging LED
constexpr uint8_t LED_RED   = 8;
constexpr uint8_t LED_GREEN = 9;
constexpr uint8_t LED_BLUE  = 10;

// Power LED
constexpr uint8_t RED_CH   = 11;
constexpr uint8_t GREEN_CH = 12;
constexpr uint8_t BLUE_CH  = 13;

constexpr uint16_t PWM_FREQ = 5000;
constexpr uint8_t PWM_RES   = 8;

// UART Link
constexpr uint8_t LINK_RX_PIN = 7;
constexpr uint8_t LINK_TX_PIN = 8;

// Wake line
constexpr uint8_t WAKE_OUT_PIN = 36;

// Comment this out to disable UART communication
#define USE_UART_LINK

#endif

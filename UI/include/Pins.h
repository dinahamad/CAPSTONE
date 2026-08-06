#ifndef PINS_H
#define PINS_H

// Main
constexpr uint8_t BUTTON_PIN   = 37;
constexpr uint8_t USB_VBUS_PIN = 36;
constexpr uint8_t USB_VBUS_PIN = 36;

// Battery
constexpr uint8_t VBAT_PIN = A13;

constexpr uint8_t CHARGE_LED1 = 12;
constexpr uint8_t CHARGE_LED2 = 27;
constexpr uint8_t CHARGE_LED3 = 33;

// Charging LED
constexpr uint8_t LED_RED   = 15;
constexpr uint8_t LED_GREEN = 32;
constexpr uint8_t LED_BLUE  = 14;

// Power LED
constexpr uint8_t RED_CH   = 0;
constexpr uint8_t GREEN_CH = 1;
constexpr uint8_t BLUE_CH  = 2;

constexpr uint16_t PWM_FREQ = 5000;
constexpr uint8_t PWM_RES   = 8;

// UART Link
constexpr uint8_t LINK_RX_PIN = 7;
constexpr uint8_t LINK_TX_PIN = 8;

// Comment this out to disable UART communication
#define USE_UART_LINK

#endif

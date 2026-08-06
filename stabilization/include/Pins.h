#ifndef PINS_H
#define PINS_H

// Main
constexpr uint8_t USB_VBUS_PIN = 36;

// Battery
constexpr uint8_t VBAT_PIN = A13;

constexpr uint16_t PWM_FREQ = 5000;
constexpr uint8_t PWM_RES   = 8;

// UART Link
constexpr uint8_t LINK_RX_PIN = 7;
constexpr uint8_t LINK_TX_PIN = 8;

// Wake line
constexpr uint8_t WAKE_IN_PIN = 33;

// Comment this out to disable UART communication
#define USE_UART_LINK

#endif
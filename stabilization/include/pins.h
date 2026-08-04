// Didi Code

#pragma once

// ============================================================
// NUMBER OF IMUs
// ============================================================

#define NUM_IMUS 2
#define NUM_SERVOS 2

// ============================================================
// SPI PINS
// ============================================================

#define SCK_PIN 5
#define MISO_PIN 21
#define MOSI_PIN 19

// ============================================================
// CHIP SELECT PINS
// ============================================================

#define CS_HAND 26
#define CS_FOREARM 25

// ============================================================
// SERVO PINS
// ============================================================

#define TOP_SERVO_PIN 27
#define BOTTOM_SERVO_PIN 33

// ============================================================
// BLE UUIDS
// ============================================================

#define SERVICE_UUID      "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define DATA_CHAR_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define COMMAND_CHAR_UUID "6e400002-b5a3-f393-e0a9-e50e24dcca9e"
#define STATUS_CHAR_UUID  "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

// ============================================================
// FILTER
// ============================================================

#define ALPHA 0.98f

// UART Link
#define LINK_RX_PIN 7;
#define LINK_TX_PIN 8;

// Battery percentage
#define USB_VBUS_PIN 36;

// Comment this out to disable UART communication
#define USE_UART_LINK

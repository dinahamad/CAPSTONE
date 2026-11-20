// ============================================================
// Wearable Stabilization Device for Tremors
//
// File: pins.h
//
// Description:
// Hardware pin assignments and system constants.
// ============================================================

#pragma once

#include <Arduino.h>

// Comment this out to disable UART communication
#define USE_UART_LINK

// ============================================================
// PIN DEFINITIONS
// ============================================================

constexpr uint16_t PWM_FREQ = 5000;
constexpr uint8_t PWM_RES   = 8;

// UART Link
constexpr uint8_t LINK_RX_PIN = 8;
constexpr uint8_t LINK_TX_PIN = 7;

// Wake line
constexpr uint8_t WAKE_IN_PIN = 25;

constexpr uint8_t SCK_PIN  = 14;
constexpr uint8_t MISO_PIN = 20;
constexpr uint8_t MOSI_PIN = 27;

// SPI PINS
constexpr uint8_t CS_HAND    = 4;
constexpr uint8_t CS_FOREARM = 26;

// SERVO PINS
constexpr uint8_t TOP_SERVO_PIN    = 37;
constexpr uint8_t BOTTOM_SERVO_PIN = 38;

// ============================================================
// IMU CONFIGURATION
// ============================================================

constexpr uint8_t NUM_IMUS = 2;

constexpr uint8_t HAND_IMU    = 0;
constexpr uint8_t FOREARM_IMU = 1;

// ============================================================
// ORIENTATION FILTER
// ============================================================

constexpr float SAMPLE_RATE = 100.0f;
constexpr float DT          = 1.0f / SAMPLE_RATE;

constexpr float ALPHA = 0.98f;

// ============================================================
// TREMOR DETECTION SETTINGS
// ============================================================

// Tremor frequency range (Hz)
constexpr float TREMOR_LOW_HZ = 4.0f;
constexpr float TREMOR_HIGH_HZ = 8.0f;

// Envelope detector
constexpr float ENVELOPE_ALPHA = 0.11f;

// Minimum envelope amplitude to classify as tremor
constexpr float TREMOR_THRESHOLD = 0.50f;

// Peak detector thresholds
constexpr float PEAK_THRESHOLD = 0.10f;
constexpr float PEAK_RESET_THRESHOLD = 0.05f;

// Expected peak periods (ms)
// 8 Hz = 125 ms
// 4 Hz = 250 ms
constexpr uint16_t MIN_PEAK_PERIOD_MS = 100;
constexpr uint16_t MAX_PEAK_PERIOD_MS = 300;

// Low-pass smoothing of measured frequency
constexpr float FREQUENCY_ALPHA = 0.20f;



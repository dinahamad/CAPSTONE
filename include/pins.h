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

// ============================================================
// IMU CONFIGURATION
// ============================================================

constexpr uint8_t NUM_IMUS = 2;

constexpr uint8_t HAND_IMU    = 0;
constexpr uint8_t FOREARM_IMU = 1;

// ============================================================
// SPI PINS
// ============================================================

constexpr uint8_t SCK_PIN  = 5;
constexpr uint8_t MISO_PIN = 21;
constexpr uint8_t MOSI_PIN = 19;

constexpr uint8_t CS_HAND    = 26;
constexpr uint8_t CS_FOREARM = 25;

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
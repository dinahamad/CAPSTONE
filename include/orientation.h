// ============================================================
// Wearable Stabilization Device for Tremors
//
// File: orientation.h
//
// Description:
// Calculates the pitch angle of each IMU using a
// complementary filter and computes the relative
// wrist pitch.
//
// Authors:
// - Didi Dimitrova
// ============================================================

#pragma once

#include <Arduino.h>

#include "imu.h"
#include "pins.h"

// Calculate pitch from accelerometer
float calculateAccelPitch(float ax, float ay, float az);

// Update the complementary filter for one IMU
bool updateOrientation(uint8_t index);

// Get wrist pitch (hand relative to forearm)
float getRelativePitch();

// Relative wrist angular velocity about pitch axis
float getRelativePitchRate();
// ============================================================
// Wearable Stabilization Device for Tremors
//
// File: calibration.h
//
// Description:
// Stores the neutral wrist position so that the
// relative wrist angle is zero in the user's
// comfortable resting position.
//
// Authors:
// - Didi Dimitrova
// ============================================================

#pragma once

// Save the current wrist position as neutral
void calibrate();

// Return the calibrated wrist pitch
float getCalibratedPitch();

float getNeutralPitch();
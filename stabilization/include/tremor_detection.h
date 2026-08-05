#pragma once

#include <Arduino.h>

#ifndef TREMOR_SERVO_1_PIN
#define TREMOR_SERVO_1_PIN 8
#endif

#ifndef TREMOR_SERVO_2_PIN
#define TREMOR_SERVO_2_PIN 9
#endif

namespace TremorControl
{
    struct Settings
    {
        // Tremor band used for the up/down wrist test.
        float highPassHz = 3.0f;
        float lowPassHz = 12.0f;
        float envelopeHz = 3.0f;

        // Amplitude hysteresis.
        float detectOnDeg = 0.65f;
        float detectOffDeg = 0.35f;

        // A tremor must repeatedly alternate direction in this frequency range.
        float minTremorHz = 3.0f;
        float maxTremorHz = 12.0f;
        float zeroCrossDeadbandDeg = 0.12f;
        uint8_t requiredValidHalfCycles = 6; // 3 complete cycles
        uint32_t oscillationTimeoutMs = 350;
        uint32_t detectOffHoldMs = 350;

        float kp = 2.2f;
        float ki = 0.35f;
        float kd = 0.035f;

        float servo1CenterDeg = 90.0f;
        float servo2CenterDeg = 90.0f;
        float servo1Direction = 1.0f;
        float servo2Direction = -1.0f;
        float maxCorrectionDeg = 8.0f;
        float maxServoRateDegPerSec = 180.0f;
        int minPulseUs = 500;
        int maxPulseUs = 2500;
        bool actuatorEnabled = false;
    };

    struct State
    {
        bool enabled;
        bool tremorDetected;
        bool frequencyValid;
        float wristPitchDeg;
        float voluntaryPitchDeg;
        float tremorPitchDeg;
        float tremorEnvelopeDeg;
        float estimatedFrequencyHz;
        uint8_t validHalfCycles;
        float pidCorrectionDeg;
        float servo1CommandDeg;
        float servo2CommandDeg;
    };

    void begin(const Settings &settings = Settings());
    void enable();
    void disable();
    void reset(float currentWristPitchDeg = 0.0f);
    void update(float relativeWristPitchDeg, uint64_t timestampUs);
    void setPID(float kp, float ki, float kd);
    const State &getState();
}

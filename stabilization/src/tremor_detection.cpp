#include "tremor_detection_improved.h"
#include <ESP32Servo.h>
#include <math.h>

namespace TremorControl
{
    namespace
    {
        Servo servo1;
        Servo servo2;
        Settings cfg;
        State state = {};

        bool servosAttached = false;
        uint64_t lastUpdateUs = 0;
        uint32_t belowSinceMs = 0;
        uint32_t lastCrossingMs = 0;
        int8_t lastStableSign = 0;

        float previousInputDeg = 0.0f;
        float highPassStateDeg = 0.0f;
        float bandPassStateDeg = 0.0f;
        float envelopeStateDeg = 0.0f;
        float integral = 0.0f;
        float previousError = 0.0f;
        float servo1PositionDeg = 90.0f;
        float servo2PositionDeg = 90.0f;

        float clampf(float value, float low, float high)
        {
            return value < low ? low : (value > high ? high : value);
        }

        float lowPassAlpha(float cutoffHz, float dt)
        {
            if (cutoffHz <= 0.0f) return 1.0f;
            const float rc = 1.0f / (2.0f * PI * cutoffHz);
            return dt / (rc + dt);
        }

        float highPassAlpha(float cutoffHz, float dt)
        {
            if (cutoffHz <= 0.0f) return 0.0f;
            const float rc = 1.0f / (2.0f * PI * cutoffHz);
            return rc / (rc + dt);
        }

        float rateLimit(float target, float current, float maxRate, float dt)
        {
            const float maxStep = maxRate * dt;
            return current + clampf(target - current, -maxStep, maxStep);
        }

        int8_t stableSign(float value)
        {
            if (value > cfg.zeroCrossDeadbandDeg) return 1;
            if (value < -cfg.zeroCrossDeadbandDeg) return -1;
            return 0;
        }

        void clearOscillationState()
        {
            lastCrossingMs = 0;
            lastStableSign = 0;
            state.validHalfCycles = 0;
            state.estimatedFrequencyHz = 0.0f;
            state.frequencyValid = false;
        }

        void writeNeutral()
        {
            servo1PositionDeg = cfg.servo1CenterDeg;
            servo2PositionDeg = cfg.servo2CenterDeg;
            state.servo1CommandDeg = servo1PositionDeg;
            state.servo2CommandDeg = servo2PositionDeg;

            if (servosAttached)
            {
                servo1.write((int)roundf(servo1PositionDeg));
                servo2.write((int)roundf(servo2PositionDeg));
            }
        }
    }

    void begin(const Settings &settings)
    {
        cfg = settings;
        servosAttached = false;

        if (cfg.actuatorEnabled)
        {
            servo1.setPeriodHertz(50);
            servo2.setPeriodHertz(50);
            servosAttached = servo1.attach(TREMOR_SERVO_1_PIN, cfg.minPulseUs, cfg.maxPulseUs) > 0;
            servosAttached = (servo2.attach(TREMOR_SERVO_2_PIN, cfg.minPulseUs, cfg.maxPulseUs) > 0) && servosAttached;
        }

        state.enabled = false;
        reset(0.0f);
        writeNeutral();
    }

    void enable()
    {
        state.enabled = true;
        integral = 0.0f;
        previousError = 0.0f;
        belowSinceMs = 0;
        clearOscillationState();
    }

    void disable()
    {
        state.enabled = false;
        state.tremorDetected = false;
        state.pidCorrectionDeg = 0.0f;
        integral = 0.0f;
        previousError = 0.0f;
        belowSinceMs = 0;
        clearOscillationState();
        writeNeutral();
    }

    void reset(float currentWristPitchDeg)
    {
        lastUpdateUs = 0;
        previousInputDeg = currentWristPitchDeg;
        highPassStateDeg = 0.0f;
        bandPassStateDeg = 0.0f;
        envelopeStateDeg = 0.0f;
        integral = 0.0f;
        previousError = 0.0f;
        belowSinceMs = 0;
        clearOscillationState();

        state.tremorDetected = false;
        state.wristPitchDeg = currentWristPitchDeg;
        state.voluntaryPitchDeg = currentWristPitchDeg;
        state.tremorPitchDeg = 0.0f;
        state.tremorEnvelopeDeg = 0.0f;
        state.pidCorrectionDeg = 0.0f;
    }

    void update(float relativeWristPitchDeg, uint64_t timestampUs)
    {
        state.wristPitchDeg = relativeWristPitchDeg;

        if (lastUpdateUs == 0)
        {
            lastUpdateUs = timestampUs;
            previousInputDeg = relativeWristPitchDeg;
            state.voluntaryPitchDeg = relativeWristPitchDeg;
            return;
        }

        const float dt = (timestampUs - lastUpdateUs) * 1.0e-6f;
        lastUpdateUs = timestampUs;

        if (dt <= 0.0f || dt > 0.05f)
        {
            reset(relativeWristPitchDeg);
            return;
        }

        const float hpA = highPassAlpha(cfg.highPassHz, dt);
        highPassStateDeg = hpA * (highPassStateDeg + relativeWristPitchDeg - previousInputDeg);
        previousInputDeg = relativeWristPitchDeg;

        const float lpA = lowPassAlpha(cfg.lowPassHz, dt);
        bandPassStateDeg += lpA * (highPassStateDeg - bandPassStateDeg);

        const float envA = lowPassAlpha(cfg.envelopeHz, dt);
        envelopeStateDeg += envA * (fabsf(bandPassStateDeg) - envelopeStateDeg);

        state.tremorPitchDeg = bandPassStateDeg;
        state.tremorEnvelopeDeg = envelopeStateDeg;
        state.voluntaryPitchDeg = relativeWristPitchDeg - bandPassStateDeg;

        const uint32_t nowMs = millis();
        const int8_t signNow = stableSign(bandPassStateDeg);

        // Count alternating half-cycles only when their timing corresponds to
        // the configured tremor-frequency range. A single slow movement does
        // not produce enough valid reversals to satisfy this requirement.
        if (signNow != 0 && signNow != lastStableSign)
        {
            if (lastStableSign != 0 && lastCrossingMs != 0)
            {
                const uint32_t halfPeriodMs = nowMs - lastCrossingMs;
                const float minHalfPeriodMs = 500.0f / cfg.maxTremorHz;
                const float maxHalfPeriodMs = 500.0f / cfg.minTremorHz;

                if (halfPeriodMs >= minHalfPeriodMs && halfPeriodMs <= maxHalfPeriodMs)
                {
                    if (state.validHalfCycles < 255) state.validHalfCycles++;
                    state.estimatedFrequencyHz = 500.0f / (float)halfPeriodMs;
                }
                else
                {
                    state.validHalfCycles = 0;
                    state.estimatedFrequencyHz = 0.0f;
                }
            }

            lastStableSign = signNow;
            lastCrossingMs = nowMs;
        }

        if (lastCrossingMs != 0 && nowMs - lastCrossingMs > cfg.oscillationTimeoutMs)
        {
            clearOscillationState();
        }

        state.frequencyValid =
            state.validHalfCycles >= cfg.requiredValidHalfCycles &&
            state.estimatedFrequencyHz >= cfg.minTremorHz &&
            state.estimatedFrequencyHz <= cfg.maxTremorHz;

        const bool tremorEvidence =
            envelopeStateDeg >= cfg.detectOnDeg && state.frequencyValid;

        if (!state.tremorDetected)
        {
            if (tremorEvidence)
            {
                state.tremorDetected = true;
                belowSinceMs = 0;
            }
        }
        else
        {
            const bool shouldTurnOff =
                envelopeStateDeg <= cfg.detectOffDeg || !state.frequencyValid;

            if (shouldTurnOff)
            {
                if (belowSinceMs == 0) belowSinceMs = nowMs;
                else if (nowMs - belowSinceMs >= cfg.detectOffHoldMs)
                {
                    state.tremorDetected = false;
                    belowSinceMs = 0;
                    integral = 0.0f;
                }
            }
            else
            {
                belowSinceMs = 0;
            }
        }

        float correctionDeg = 0.0f;
        if (state.enabled && state.tremorDetected)
        {
            const float error = -bandPassStateDeg;
            const float derivative = (error - previousError) / dt;
            integral += error * dt;
            const float integralLimit = cfg.maxCorrectionDeg / fmaxf(cfg.ki, 0.001f);
            integral = clampf(integral, -integralLimit, integralLimit);
            correctionDeg = cfg.kp * error + cfg.ki * integral + cfg.kd * derivative;
            correctionDeg = clampf(correctionDeg, -cfg.maxCorrectionDeg, cfg.maxCorrectionDeg);
            previousError = error;
        }
        else
        {
            integral = 0.0f;
            previousError = 0.0f;
        }

        state.pidCorrectionDeg = correctionDeg;

        const float target1 = clampf(cfg.servo1CenterDeg + cfg.servo1Direction * correctionDeg, 0.0f, 180.0f);
        const float target2 = clampf(cfg.servo2CenterDeg + cfg.servo2Direction * correctionDeg, 0.0f, 180.0f);

        servo1PositionDeg = rateLimit(target1, servo1PositionDeg, cfg.maxServoRateDegPerSec, dt);
        servo2PositionDeg = rateLimit(target2, servo2PositionDeg, cfg.maxServoRateDegPerSec, dt);

        if (servosAttached)
        {
            servo1.write((int)roundf(servo1PositionDeg));
            servo2.write((int)roundf(servo2PositionDeg));
        }

        state.servo1CommandDeg = servo1PositionDeg;
        state.servo2CommandDeg = servo2PositionDeg;
    }

    void setPID(float kp, float ki, float kd)
    {
        cfg.kp = kp;
        cfg.ki = ki;
        cfg.kd = kd;
        integral = 0.0f;
        previousError = 0.0f;
    }

    const State &getState()
    {
        return state;
    }
}

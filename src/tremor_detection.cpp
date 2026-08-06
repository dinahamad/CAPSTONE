// // ============================================================
// // Wearable Stabilization Device for Tremors
// //
// // File: tremor_detection.cpp
// //
// // Description:
// // Real-time tremor detection using:
// //
// // Relative Wrist Pitch
// //      -> 4-8 Hz Butterworth Band-pass
// //      -> Envelope Detector
// //      -> FFT Buffer (placeholder)
// //      -> Monitoring / Tracking
// // ============================================================

// #include "tremor_detection.h"
// #include <Arduino.h>
// #include <math.h>

// // ============================================================
// // MATLAB FILTER COEFFICIENTS
// // butter(2,[4 8]/(100/2),'bandpass')
// // ============================================================

// static const float b[5] =
// {
//     0.0134f,
//     0.0000f,
//    -0.0267f,
//     0.0000f,
//     0.0134f
// };

// static const float a[5] =
// {
//     1.0000f,
//    -3.4183f,
//     4.5883f,
//    -2.8577f,
//     0.7009f
// };

// // ============================================================
// // SETTINGS
// // ============================================================

// static constexpr float ENVELOPE_ALPHA = 0.11f;
// static constexpr float TREMOR_THRESHOLD = 0.50f;
// static constexpr int FFT_SIZE = 200;

// // ============================================================
// // FILTER MEMORY
// // ============================================================

// static float x[5] = {0};
// static float y[5] = {0};

// // ============================================================
// // FFT BUFFER
// // ============================================================

// static float fftBuffer[FFT_SIZE];
// static int fftIndex = 0;

// // ============================================================
// // GLOBAL ESTIMATE
// // ============================================================

// static TremorEstimate tremor;

// // ============================================================

// void initializeTremorDetection()
// {
//     for(int i=0;i<5;i++)
//     {
//         x[i]=0.0f;
//         y[i]=0.0f;
//     }

//     for(int i=0;i<FFT_SIZE;i++)
//         fftBuffer[i]=0.0f;

//     fftIndex = 0;

//     tremor.handPitch = 0.0f;
//     tremor.forearmPitch = 0.0f;
//     tremor.wristPitch = 0.0f;
//     tremor.tremorPitch = 0.0f;
//     tremor.envelope = 0.0f;
//     tremor.frequency = 0.0f;
//     tremor.confidence = 0.0f;
//     tremor.state = MONITORING;
// }

// void updateBandPass(float sample)
// {
//     x[4]=x[3];
//     x[3]=x[2];
//     x[2]=x[1];
//     x[1]=x[0];
//     x[0]=sample;

//     float output =
//         b[0]*x[0] +
//         b[1]*x[1] +
//         b[2]*x[2] +
//         b[3]*x[3] +
//         b[4]*x[4]
//         -a[1]*y[0]
//         -a[2]*y[1]
//         -a[3]*y[2]
//         -a[4]*y[3];

//     y[3]=y[2];
//     y[2]=y[1];
//     y[1]=y[0];
//     y[0]=output;

//     tremor.tremorPitch = output;
// }

// void updateEnvelope()
// {
//     float magnitude = fabsf(tremor.tremorPitch);

//     tremor.envelope +=
//         ENVELOPE_ALPHA *
//         (magnitude - tremor.envelope);
// }

// void updateFFT()
// {
//     fftBuffer[fftIndex] = tremor.tremorPitch;
//     fftIndex++;

//     if(fftIndex >= FFT_SIZE)
//         fftIndex = 0;

//     // Placeholder until FFT implementation.
//     tremor.frequency = 0.0f;
// }

// void updateStateMachine()
// {
//     tremor.confidence =
//         constrain((tremor.envelope/TREMOR_THRESHOLD)*100.0f,
//                   0.0f,100.0f);

//     if(tremor.envelope >= TREMOR_THRESHOLD)
//         tremor.state = TREMOR_DETECTED;
//     else
//         tremor.state = MONITORING;
// }

// void updateTremorDetection(
//     float handPitch,
//     float forearmPitch,
//     float wristPitch)
// {
//     tremor.handPitch = handPitch;

//     tremor.forearmPitch = forearmPitch;

//     tremor.wristPitch = wristPitch;

//     updateBandPass(wristPitch);

//     updateEnvelope();

//     updateFFT();

//     updateStateMachine();
// }

// float getRawPitch(){return tremor.wristPitch;}
// float getFilteredPitch(){ return tremor.tremorPitch; }
// float getEnvelope(){ return tremor.envelope; }
// float getDominantFrequency(){ return tremor.frequency; }
// float getDetectionConfidence(){ return tremor.confidence; }
// TremorState getTremorState(){ return tremor.state; }
// bool tremorDetected(){ return tremor.state==TREMOR_DETECTED; }
// const TremorEstimate& getTremorEstimate(){ return tremor; }

// void printTremorData()
// {
//     Serial.print("Hand Pitch: ");
//     Serial.print(tremor.handPitch, 2);

//     Serial.print("° | Forearm Pitch: ");
//     Serial.print(tremor.forearmPitch, 2);

//     Serial.print("° | Wrist Pitch: ");
//     Serial.print(tremor.wristPitch, 2);

//     Serial.print("° | Tremor Pitch: ");
//     Serial.print(tremor.tremorPitch, 2);

//     Serial.print("° | Envelope: ");
//     Serial.print(tremor.envelope, 2);

//     Serial.print("° | Frequency: ");
//     Serial.print(tremor.frequency, 2);

//     Serial.print(" Hz | Confidence: ");
//     Serial.print(tremor.confidence, 1);

//     Serial.print("% | Status: ");

//     if (tremor.state == TREMOR_DETECTED)
//     {
//         Serial.println("Tremor Detected");
//     }
//     else
//     {
//         Serial.println("Monitoring");
//     }
// }

// ============================================================
// Wearable Stabilization Device for Tremors
//
// File: tremor_detection.cpp
//
// Description:
// Real-time tremor detection using:
//
// Relative Wrist Pitch
//      -> 4–8 Hz Butterworth Band-pass
//      -> Envelope Detector
//      -> State Machine Peak Detector
//      -> Tremor Classification
// ============================================================

#include "tremor_detection.h"
#include "pins.h"

#include <Arduino.h>
#include <math.h>

// ============================================================
// MATLAB FILTER COEFFICIENTS
// butter(2,[4 8]/(100/2),'bandpass')
// ============================================================

static const float b[5] =
{
    0.0134f,
    0.0000f,
   -0.0267f,
    0.0000f,
    0.0134f
};

static const float a[5] =
{
    1.0000f,
   -3.4183f,
    4.5883f,
   -2.8577f,
    0.7009f
};

// ============================================================
// FILTER MEMORY
// ============================================================

static float x[5] = {0.0f};
static float y[5] = {0.0f};

// ============================================================
// PEAK DETECTOR VARIABLES
// ============================================================

enum PeakState
{
    WAITING_FOR_RISE,
    TRACKING_PEAK,
    WAITING_FOR_RESET
};

static PeakState peakState = WAITING_FOR_RISE;

static float previousSample = 0.0f;
static float currentPeak = 0.0f;

static uint32_t previousPeakTime = 0;

// ============================================================
// GLOBAL TREMOR ESTIMATE
// ============================================================

static TremorEstimate tremor;

// ============================================================
// INITIALIZATION
// ============================================================

void initializeTremorDetection()
{
    for(int i = 0; i < 5; i++)
    {
        x[i] = 0.0f;
        y[i] = 0.0f;
    }

    peakState = WAITING_FOR_RISE;

    previousSample = 0.0f;
    currentPeak = 0.0f;
    previousPeakTime = 0;

    tremor.handPitch = 0.0f;
    tremor.forearmPitch = 0.0f;
    tremor.wristPitch = 0.0f;
    tremor.tremorPitch = 0.0f;

    tremor.envelope = 0.0f;
    tremor.frequency = 0.0f;
    tremor.confidence = 0.0f;

    tremor.state = MONITORING;
}

// ============================================================
// 4–8 Hz BUTTERWORTH FILTER
// ============================================================

void updateBandPass(float sample)
{
    x[4] = x[3];
    x[3] = x[2];
    x[2] = x[1];
    x[1] = x[0];
    x[0] = sample;

    float output =
        b[0]*x[0] +
        b[1]*x[1] +
        b[2]*x[2] +
        b[3]*x[3] +
        b[4]*x[4]
        -a[1]*y[0]
        -a[2]*y[1]
        -a[3]*y[2]
        -a[4]*y[3];

    y[3] = y[2];
    y[2] = y[1];
    y[1] = y[0];
    y[0] = output;

    tremor.tremorPitch = output;
}

// ============================================================
// ENVELOPE DETECTOR
// ============================================================

void updateEnvelope()
{
    float magnitude = fabsf(tremor.tremorPitch);

    tremor.envelope +=
        ENVELOPE_ALPHA *
        (magnitude - tremor.envelope);
}

// ============================================================
// STATE MACHINE PEAK DETECTOR
// ============================================================

void updatePeakFrequency()
{
    float sample = tremor.tremorPitch;

    switch (peakState)
    {

    //--------------------------------------------------------
    // WAIT FOR THE SIGNAL TO RISE ABOVE THE PEAK THRESHOLD
    //--------------------------------------------------------

    case WAITING_FOR_RISE:

        if (sample > PEAK_THRESHOLD)
        {
            currentPeak = sample;
            peakState = TRACKING_PEAK;
        }

        break;

    //--------------------------------------------------------
    // TRACK THE HIGHEST POINT OF THE PEAK
    //--------------------------------------------------------

    case TRACKING_PEAK:

        if (sample > currentPeak)
        {
            currentPeak = sample;
        }

        // Signal has started falling -> peak found
        if (sample < previousSample)
        {
            uint32_t now = millis();

            if (previousPeakTime != 0)
            {
                uint32_t period =
                    now - previousPeakTime;

                if (period >= MIN_PEAK_PERIOD_MS &&
                    period <= MAX_PEAK_PERIOD_MS)
                {
                    float measuredFrequency =
                        1000.0f /
                        (float)period;

                    // Smooth frequency estimate
                    tremor.frequency +=
                        FREQUENCY_ALPHA *
                        (measuredFrequency -
                         tremor.frequency);
                }
            }

            previousPeakTime = now;

            peakState = WAITING_FOR_RESET;
        }

        break;

    //--------------------------------------------------------
    // WAIT UNTIL THE SIGNAL HAS FALLEN LOW ENOUGH
    // BEFORE SEARCHING FOR THE NEXT PEAK
    //--------------------------------------------------------

    case WAITING_FOR_RESET:

        if (sample < PEAK_RESET_THRESHOLD)
        {
            peakState = WAITING_FOR_RISE;
        }

        break;
    }

    //--------------------------------------------------------
    // RESET FREQUENCY IF NO PEAKS ARE DETECTED
    //--------------------------------------------------------

    if (previousPeakTime != 0 &&
        millis() - previousPeakTime > 500)
    {
        tremor.frequency = 0.0f;
    }

    previousSample = sample;
}

// ============================================================
// STATE MACHINE
// ============================================================

void updateStateMachine()
{
    tremor.confidence =
        constrain(
            (tremor.envelope / TREMOR_THRESHOLD) * 100.0f,
            0.0f,
            100.0f);

    bool validAmplitude =
        tremor.envelope >= TREMOR_THRESHOLD;

    bool validFrequency =
        tremor.frequency >= TREMOR_LOW_HZ &&
        tremor.frequency <= TREMOR_HIGH_HZ;

    if (validAmplitude &&
        validFrequency)
    {
        tremor.state = TREMOR_DETECTED;
    }
    else
    {
        tremor.state = MONITORING;
    }
}

// ============================================================
// MAIN UPDATE
// ============================================================

void updateTremorDetection(
    float handPitch,
    float forearmPitch,
    float wristPitch)
{
    // Save latest orientation estimates
    tremor.handPitch = handPitch;
    tremor.forearmPitch = forearmPitch;
    tremor.wristPitch = wristPitch;

    // Processing pipeline
    updateBandPass(wristPitch);

    updateEnvelope();

    updatePeakFrequency();

    updateStateMachine();
}

// ============================================================
// GETTERS
// ============================================================

float getRawPitch()
{
    return tremor.wristPitch;
}

float getFilteredPitch()
{
    return tremor.tremorPitch;
}

float getEnvelope()
{
    return tremor.envelope;
}

float getDominantFrequency()
{
    return tremor.frequency;
}

float getDetectionConfidence()
{
    return tremor.confidence;
}

TremorState getTremorState()
{
    return tremor.state;
}

bool tremorDetected()
{
    return tremor.state == TREMOR_DETECTED;
}

const TremorEstimate& getTremorEstimate()
{
    return tremor;
}

// ============================================================
// DEBUG OUTPUT
// ============================================================

void printTremorData()
{
    Serial.print("Hand Pitch: ");
    Serial.print(tremor.handPitch, 2);

    Serial.print("° | Forearm Pitch: ");
    Serial.print(tremor.forearmPitch, 2);

    Serial.print("° | Wrist Pitch: ");
    Serial.print(tremor.wristPitch, 2);

    Serial.print("° | Tremor Pitch: ");
    Serial.print(tremor.tremorPitch, 2);

    Serial.print("° | Envelope: ");
    Serial.print(tremor.envelope, 2);

    Serial.print("° | Frequency: ");
    Serial.print(tremor.frequency, 2);

    Serial.print(" Hz | Confidence: ");
    Serial.print(tremor.confidence, 1);

    Serial.print("% | Status: ");

    if (tremor.state == TREMOR_DETECTED)
    {
        Serial.println("Tremor Detected");
    }
    else
    {
        Serial.println("Monitoring");
    }
}
// ============================================================
// Wearable Stabilization Device for Tremors
//
// File: orientation.cpp
//
// Description:
// Calculates the pitch angle of each IMU using a
// complementary filter.
//
// Authors:
// - Didi Dimitrova
// ============================================================

#include "orientation.h"
#include "tremor_detection.h"
#include "calibration.h"

#include <math.h>

// ============================================================
// ACCELEROMETER PITCH
// ============================================================

float calculateAccelPitch(float ax, float ay, float az)
{
    return atan2(-ax, sqrt(ay * ay + az * az)) * RAD_TO_DEG;
}

// ============================================================
// UPDATE ORIENTATION
// ============================================================

bool updateOrientation(uint8_t index)
{
    // Read newest IMU data
    if (!readIMU(index))
    {
        return false;
    }

    // Pitch from accelerometer
    float accelPitch =
        calculateAccelPitch(
            imuData[index].ax,
            imuData[index].ay,
            imuData[index].az);

    // Integrate gyro
    float gyroPitch =
        imuData[index].pitch +
        imuData[index].gy * DT;

    // Complementary filter
    imuData[index].pitch =
        ALPHA * gyroPitch +
        (1.0f - ALPHA) * accelPitch;

    return true;
}

// ============================================================
// RELATIVE WRIST PITCH
// ============================================================

float getRelativePitch()
{
    return imuData[HAND_IMU].pitch -
           imuData[FOREARM_IMU].pitch;
}

// ============================================================
// RELATIVE WRIST PITCH RATE
// ============================================================

float getRelativePitchRate()
{
    return imuData[HAND_IMU].gy -
           imuData[FOREARM_IMU].gy;
}



// // ============================================================
// // MADGWICK IMU ORIENTATION UPDATE
// // ============================================================

// static constexpr float MADGWICK_BETA = 0.08f;

// bool updateOrientation(uint8_t index)
// {
//     if (!readIMU(index))
//     {
//         return false;
//     }

//     // --------------------------------------------------------
//     // Current quaternion
//     // --------------------------------------------------------

//     float q0 = imuData[index].qw;
//     float q1 = imuData[index].qx;
//     float q2 = imuData[index].qy;
//     float q3 = imuData[index].qz;

//     // --------------------------------------------------------
//     // Gyroscope
//     //
//     // ICM-20948 gives deg/s.
//     // Madgwick requires rad/s.
//     // --------------------------------------------------------

//     float gx = imuData[index].gx * DEG_TO_RAD;
//     float gy = imuData[index].gy * DEG_TO_RAD;
//     float gz = imuData[index].gz * DEG_TO_RAD;

//     // --------------------------------------------------------
//     // Accelerometer
//     // --------------------------------------------------------

//     float ax = imuData[index].ax;
//     float ay = imuData[index].ay;
//     float az = imuData[index].az;

//     // ========================================================
//     // Quaternion derivative from gyroscope
//     // ========================================================

//     float qDot0 =
//         0.5f * (-q1 * gx - q2 * gy - q3 * gz);

//     float qDot1 =
//         0.5f * ( q0 * gx + q2 * gz - q3 * gy);

//     float qDot2 =
//         0.5f * ( q0 * gy - q1 * gz + q3 * gx);

//     float qDot3 =
//         0.5f * ( q0 * gz + q1 * gy - q2 * gx);

//     // ========================================================
//     // Accelerometer correction
//     // ========================================================

//     float accelNorm =
//         sqrtf(ax * ax +
//               ay * ay +
//               az * az);

//     if (accelNorm > 0.0001f)
//     {
//         // Normalize accelerometer
//         ax /= accelNorm;
//         ay /= accelNorm;
//         az /= accelNorm;

//         float _2q0 = 2.0f * q0;
//         float _2q1 = 2.0f * q1;
//         float _2q2 = 2.0f * q2;
//         float _2q3 = 2.0f * q3;

//         float _4q0 = 4.0f * q0;
//         float _4q1 = 4.0f * q1;
//         float _4q2 = 4.0f * q2;

//         float _8q1 = 8.0f * q1;
//         float _8q2 = 8.0f * q2;

//         float q0q0 = q0 * q0;
//         float q1q1 = q1 * q1;
//         float q2q2 = q2 * q2;
//         float q3q3 = q3 * q3;

//         // Gradient descent correction
//         float s0 =
//             _4q0 * q2q2 +
//             _2q2 * ax +
//             _4q0 * q1q1 -
//             _2q1 * ay;

//         float s1 =
//             _4q1 * q3q3 -
//             _2q3 * ax +
//             4.0f * q0q0 * q1 -
//             _2q0 * ay -
//             _4q1 +
//             _8q1 * q1q1 +
//             _8q1 * q2q2 +
//             _4q1 * az;

//         float s2 =
//             4.0f * q0q0 * q2 +
//             _2q0 * ax +
//             _4q2 * q3q3 -
//             _2q3 * ay -
//             _4q2 +
//             _8q2 * q1q1 +
//             _8q2 * q2q2 +
//             _4q2 * az;

//         float s3 =
//             4.0f * q1q1 * q3 -
//             _2q1 * ax +
//             4.0f * q2q2 * q3 -
//             _2q2 * ay;

//         // Normalize correction
//         float stepNorm =
//             sqrtf(
//                 s0 * s0 +
//                 s1 * s1 +
//                 s2 * s2 +
//                 s3 * s3);

//         if (stepNorm > 0.0001f)
//         {
//             s0 /= stepNorm;
//             s1 /= stepNorm;
//             s2 /= stepNorm;
//             s3 /= stepNorm;

//             qDot0 -= MADGWICK_BETA * s0;
//             qDot1 -= MADGWICK_BETA * s1;
//             qDot2 -= MADGWICK_BETA * s2;
//             qDot3 -= MADGWICK_BETA * s3;
//         }
//     }

//     // ========================================================
//     // Integrate quaternion
//     // ========================================================

//     q0 += qDot0 * DT;
//     q1 += qDot1 * DT;
//     q2 += qDot2 * DT;
//     q3 += qDot3 * DT;

//     // ========================================================
//     // Normalize quaternion
//     // ========================================================

//     float quaternionNorm =
//         sqrtf(
//             q0 * q0 +
//             q1 * q1 +
//             q2 * q2 +
//             q3 * q3);

//     if (quaternionNorm > 0.0001f)
//     {
//         q0 /= quaternionNorm;
//         q1 /= quaternionNorm;
//         q2 /= quaternionNorm;
//         q3 /= quaternionNorm;
//     }

//     // Store orientation
//     imuData[index].qw = q0;
//     imuData[index].qx = q1;
//     imuData[index].qy = q2;
//     imuData[index].qz = q3;

//     // --------------------------------------------------------
//     // Keep pitch available for debugging/dashboard
//     // --------------------------------------------------------

//     float sinPitch =
//         2.0f * (q0 * q2 - q3 * q1);

//     sinPitch = constrain(
//         sinPitch,
//         -1.0f,
//         1.0f);

//     imuData[index].pitch =
//         asinf(sinPitch) * RAD_TO_DEG;

//     return true;
// }

// ============================================================
// UPDATE SENSING
// ============================================================

// Updates both IMU orientations and processes the current wrist motion.
bool updateSensing()
{
    // Update hand orientation
    if (!updateOrientation(HAND_IMU))
    {
        return false;
    }

    // Update forearm orientation
    if (!updateOrientation(FOREARM_IMU))
    {
        return false;
    }

    // Calculate relative wrist orientation
    float wristPitch = getCalibratedPitch();

    // Process wrist motion for tremor detection
    updateTremorDetection(
        imuData[HAND_IMU].pitch,
        imuData[FOREARM_IMU].pitch,
        wristPitch);

    return true;
}
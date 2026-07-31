#include <Arduino.h>
#include <ESP32Servo.h>
#include "servo_control.h"

// create servo objects
Servo wristPitchTopServo;
Servo wristPitchBottomServo;

void setupServos()
{
    wristPitchServo.attach(WRIST_PITCH_TOP_PIN);
    wristPitchServo.attach(WRIST_PITCH_BOTTOM_PIN);

    // Start all servos at their centre positions
    wristPitchServo.write(WRIST_PITCH_TOP_START);
    wristPitchServo.write(WRIST_PITCH_BOTTOM_START);
}

void moveWristTopPitch(int angle)
{
    angle = constrain(angle, WRIST_PITCH_TOP_MIN, WRIST_PITCH_TOP_MAX);
    wristPitchTopServo.write(angle);
}

void moveWristBottomPitch(int angle)
{
    angle = constrain(angle, WRIST_PITCH_BOTTOM_MIN, WRIST_PITCH_BOTTOM_MAX);
    wristPitchBottomServo.write(angle);
}



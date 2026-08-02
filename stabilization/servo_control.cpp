#include <Arduino.h>
#include <ESP32Servo.h>
#include "servo_control.h"

// servo objects
Servo wristPitchTopServo;
Servo wristPitchBottomServo;

void setupServos()
{
    // Connect each servo object to its pin
    wristPitchTopServo.attach(WRIST_PITCH_TOP_PIN);
    wristPitchBottomServo.attach(WRIST_PITCH_BOTTOM_PIN);

    // Put both servos at their starting positions
    stopWrist();
}

void moveHandUp(int amount)
{
    // amount must be between 0 max pull amount
    amount = constrain(amount, WRIST_PITCH_TOP_MIN, WRIST_PITCH_TOP_MAX);

    // Top servo pulls upward
    int topAngle = WRIST_PITCH_TOP_START + amount * WRIST_PITCH_TOP_DIRECTION;

    // Top servo pulls; bottom servo returns to neutral
    wristPitchTopServo.write(topAngle);
    wristPitchBottomServo.write(WRIST_PITCH_BOTTOM_START);
}

void moveHandDown(int amount)
{
    amount = constrain(amount, WRIST_PITCH_BOTTOM_MIN, WRIST_PITCH_BOTTOM_MAX);

    // Bottom servo pulls downward
    int bottomAngle = WRIST_PITCH_BOTTOM_START + amount * WRIST_PITCH_BOTTOM_DIRECTION;

    // Bottom servo pulls; top servo returns to neutral
    wristPitchTopServo.write(WRIST_PITCH_TOP_START);
    wristPitchBottomServo.write(bottomAngle);
}


void stopWrist()
{
    // Neither servo actively corrects the wrist, put back to neutral
    wristPitchTopServo.write(WRIST_PITCH_TOP_START);
    wristPitchBottomServo.write(WRIST_PITCH_BOTTOM_START);
}


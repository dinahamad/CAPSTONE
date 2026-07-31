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
    amount = constrain(amount, 0, WRIST_PITCH_TOP_PULL_RANGE);

    // Top servo pulls upward
    int topAngle = WRIST_PITCH_TOP_START + amount;

    topAngle = constrain(topAngle, WRIST_PITCH_TOP_MIN,WRIST_PITCH_TOP_MAX);

    // Top servo pulls; bottom servo returns to neutral
    wristPitchTopServo.write(topAngle);
    wristPitchBottomServo.write(WRIST_PITCH_BOTTOM_START);
}

void moveHandDown(int amount)
{
    amount = constrain(amount, 0, WRIST_PITCH_BOTTOM_PULL_RANGE);

    // Bottom servo pulls downward
    int bottomAngle = WRIST_PITCH_BOTTOM_START + amount;

    bottomAngle = constrain(bottomAngle,WRIST_PITCH_MIN,WRIST_PITCH_MAX);

    // Bottom servo pulls; top servo returns to neutral
    wristPitchTopServo.write(WRIST_PITCH_TOP_START);
    wristPitchBottomServo.write(bottomAngle);
}



void stopWrist()
{
    // Neither servo actively corrects the wrist
    wristPitchTopServo.write(WRIST_PITCH_TOP_START);
    wristPitchBottomServo.write(WRIST_PITCH_BOTTOM_START);
}


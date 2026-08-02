#include <Arduino.h>
#include <ESP32Servo.h>
#include "servo_control.h"

// servo objects
Servo wristPitchTopServo;
Servo wristPitchBottomServo;

// Current servo angles. Start at neutral
static int wristPitchTopServoCommand = WRIST_PITCH_TOP_START;
static int wristPitchBottomServoCommand =WRIST_PITCH_BOTTOM_START;

// Whether the servos are currently attached
static bool servosEnabled = false;

void setupServos()
{
    // Connect each servo object to its pin
    wristPitchTopServo.attach(WRIST_PITCH_TOP_PIN);
    wristPitchBottomServo.attach(WRIST_PITCH_BOTTOM_PIN);
    servosEnabled = true; // they are attached
    // Put both servos at their starting positions
    stopWrist();
}

// Main function called by the control algorithm
void setWristCorrection(float correction) {
    if (correction > 0.0f) {moveHandUp((int)correction);}
    else if (correction < 0.0f) {moveHandDown((int)(-correction));}
    else{stopWrist();}
}


void moveHandUp(int amount)
{
    // amount must be between 0 max pull amount
    amount = constrain(amount, WRIST_PITCH_TOP_MIN, WRIST_PITCH_TOP_MAX);

    // Top servo pulls upward
    int topAngle = amount * WRIST_PITCH_TOP_DIRECTION;

    // Top servo pulls; bottom servo returns to neutral
    wristPitchTopServo.write(topAngle);
    wristPitchBottomServo.write(WRIST_PITCH_BOTTOM_START);

    wristPitchTopServoCommand = topAngle;
}

void moveHandDown(int amount)
{
    amount = constrain(amount, WRIST_PITCH_BOTTOM_MIN, WRIST_PITCH_BOTTOM_MAX);

    // Bottom servo pulls downward
    int bottomAngle = amount * WRIST_PITCH_BOTTOM_DIRECTION;

    // Bottom servo pulls; top servo returns to neutral
    wristPitchTopServo.write(WRIST_PITCH_TOP_START);
    wristPitchBottomServo.write(bottomAngle);

    wristPitchBottomServoCommand = bottomAngle;
}


void stopWrist()
{
    // Neither servo actively corrects the wrist, put back to neutral
    wristPitchTopServo.write(WRIST_PITCH_TOP_START);
    wristPitchBottomServo.write(WRIST_PITCH_BOTTOM_START);
}


#include <Arduino.h>
#include <ESP32Servo.h>
#include "servo_control.h"
#include <iostream>

/////////////////////////////////////////////////////////////////////////////////////////////
// This file is for the servos that pull the hand up and down relative to the forearm (pitch)
/////////////////////////////////////////////////////////////////////////////////////////////

// Servo objects
Servo topServo;
Servo bottomServo;

// start at neutral, changes for evreythime it moves
int TopServoCurrentAngle = TOP_SERVO_ORIGIN_ANGLE;
int BottomServoCurrentAngle = BOTTOM_SERVO_ORIGIN_ANGLE;

// connect servos to microcontroller
void setupServos() {
    // Connect each servo object to its pin
    topServo.attach(TOP_SERVO_PIN);
    bottomServo.attach(BOTTOM_SERVO_PIN);

    // Put both servos at their starting positions
    originAngles();
    servosConnected[0] = true;
    servosConnected[1] = true;
}

// this function may be called my main.cpp
void setServoAngles(float topTargetAngle, float bottomTargetAngle) {

    int safeTopAngle = constrain(
        (int)round(topTargetAngle),
        TOP_SERVO_MIN_ANGLE,
        TOP_SERVO_MAX_ANGLE
    );

    int safeBottomAngle = constrain(
        (int)round(bottomTargetAngle),
        BOTTOM_SERVO_MIN_ANGLE,
        BOTTOM_SERVO_MAX_ANGLE
    );

    topServo.write(safeTopAngle);
    bottomServo.write(safeBottomAngle);

    TopServoCurrentAngle = safeTopAngle;
    BottomServoCurrentAngle = safeBottomAngle;
}

// FUNCTIONS FOR VOLUNTARY MOVEMENT AKA WHERE THE "CENTER" OF THE HAND IS///////////////////////////////////////////////////////////
void setWristPitchValue(float correction) {
    if (correction > 0) {moveHandUp((int)correction);}
    else if (correction < 0) {moveHandDown((int)(-correction));}
}

// both the directions need to be opposite for each command (-1 and 1 or vice versa)
// later have one variable for up one for down
void moveHandUp(int amount)
{
    // servos pulls upward with the hand
    int topAngle = TopServoCurrentAngle + amount;
    int bottomAngle = BottomServoCurrentAngle + amount;

    // constrain to the safety limit for both servos
    topAngle = constrain(topAngle, TOP_SERVO_MIN_ANGLE, TOP_SERVO_MAX_ANGLE);
    bottomAngle = constrain(bottomAngle, BOTTOM_SERVO_MIN_ANGLE, BOTTOM_SERVO_MAX_ANGLE);

    topServo.write(topAngle);
    bottomServo.write(bottomAngle);

    // update current angles
    TopServoCurrentAngle = topAngle;
    BottomServoCurrentAngle = bottomAngle;
}

void moveHandDown(int amount) {
    // servo pulls downwards with the hand
    int topAngle = TopServoCurrentAngle - amount;
    int bottomAngle = BottomServoCurrentAngle - amount;

    // constrain to the safety limit for both servos
    topAngle = constrain(topAngle, TOP_SERVO_MIN_ANGLE, TOP_SERVO_MAX_ANGLE);
    bottomAngle = constrain(bottomAngle, BOTTOM_SERVO_MIN_ANGLE, BOTTOM_SERVO_MAX_ANGLE);

    topServo.write(topAngle);
    bottomServo.write(bottomAngle);

    // update current angles
    TopServoCurrentAngle = topAngle;
    BottomServoCurrentAngle = bottomAngle;
}

// FUNCTIONS FOR TREMOR MOVEMENT/////////////////////////////////////////////////////////////////////////////////////////////







void originAngles() {
    // Neither servo actively corrects the wrist, put back to neutral
    topServo.write(TOP_SERVO_ORIGIN_ANGLE);
    bottomServo.write(BOTTOM_SERVO_ORIGIN_ANGLE);

    // update current angles
    TopServoCurrentAngle = TOP_SERVO_ORIGIN_ANGLE;
    BottomServoCurrentAngle = BOTTOM_SERVO_ORIGIN_ANGLE;   
}

void endProgramServos() {
  topServo.detach();
  bottomServo.detach();
  servosConnected[0] = false;
  servosConnected[1] = false;
}

int getTopServoAngle() {
    return(TopServoCurrentAngle);
};

int getBottomServoAngle() {
    return(BottomServoCurrentAngle);
};

bool areServosEnabled(){
    return (servosConnected[0] && servosConnected[1]);
};

#include <Arduino.h>
#include <ESP32Servo.h>
#include "servo.h"
#include <math.h>
#include "tremor_detection.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// This file is for the servos that pull the hand up and down relative to the forearm (pitch)
/////////////////////////////////////////////////////////////////////////////////////////////

// Servo objects
Servo topServo;
Servo bottomServo;

// flags for servo connections
bool topServoConnected = false;
bool bottomServoConnected = false;

// start at neutral, changes for evreythime it moves
float topServoCurrentAngle;
float bottomServoCurrentAngle;

static float currentServoOffset = 0.0f;
static uint32_t previousServoUpdateTime = 0;

// takes one wrist offset and converts it into the two opposite servo angles
// realtive to center
static void writeServoAngles(float servoOffset)
{
    int topTargetAngle = TOP_SERVO_ORIGIN_ANGLE + TOP_SERVO_DIRECTION * servoOffset;
    int bottomTargetAngle = BOTTOM_SERVO_ORIGIN_ANGLE - BOTTOM_SERVO_DIRECTION * servoOffset;

    topTargetAngle = constrain(topTargetAngle,TOP_SERVO_MIN_ANGLE,TOP_SERVO_MAX_ANGLE);

    bottomTargetAngle = constrain(bottomTargetAngle,BOTTOM_SERVO_MIN_ANGLE,BOTTOM_SERVO_MAX_ANGLE);

    if (topTargetAngle != topServoCurrentAngle) {
        topServo.write((int)roundf(topTargetAngle));
        topServoCurrentAngle = topTargetAngle;
    }

    if (bottomTargetAngle != bottomServoCurrentAngle){
        bottomServo.write((int)roundf(bottomTargetAngle));
        bottomServoCurrentAngle = bottomTargetAngle;
    }
}

// gets its info directly from tremorDetected() and getFilteredPitch()
void updateServos(float wristPitch)
{
    if (!areServosEnabled()) {return;}

    // current time of microcontroller
    uint32_t currentTime = millis();

    /*
    if (currentTime - previousServoUpdateTime < SERVO_UPDATE_INTERVAL_MS) {
        return;
    }
    */
    previousServoUpdateTime = currentTime;

    // Isolated tremor from the band-pass filter
    float tremorPitch = getFilteredPitch();

    // Full wrist motion minus tremor gives estimated voluntary movement
    float voluntaryPitch = wristPitch;// - tremorPitch;

    // Follow voluntary movement while moving against tremor
    float targetServoOffset =
        VOLUNTARY_MOVEMENT_GAIN * voluntaryPitch
        - TREMOR_OPPOSITION_GAIN * tremorPitch;

    targetServoOffset = constrain(
        targetServoOffset,
        -MAX_SERVO_OFFSET,
        MAX_SERVO_OFFSET
    );

    currentServoOffset +=
        SERVO_SMOOTHING *
        (targetServoOffset - currentServoOffset);

    writeServoAngles(currentServoOffset);
}

// connect servos to microcontroller
void setupServos() {
    // Connect each servo object to its pin
    topServo.attach(TOP_SERVO_PIN);
    bottomServo.attach(BOTTOM_SERVO_PIN);

    // Put both servos at their starting positions
    topServoConnected = true;
    bottomServoConnected = true;

    servosSlack();
    //originAngles();
}

// put servos to their origin
void originAngles() {
    currentServoOffset = 0.0f;
    topServo.write(TOP_SERVO_ORIGIN_ANGLE);
    bottomServo.write(BOTTOM_SERVO_ORIGIN_ANGLE);

    // update current angles
    topServoCurrentAngle = TOP_SERVO_ORIGIN_ANGLE;
    bottomServoCurrentAngle = BOTTOM_SERVO_ORIGIN_ANGLE;   
}

void endProgramServos() {
    topServo.detach();
    bottomServo.detach();
    topServoConnected = false;
    bottomServoConnected = false;
}

// for torubleshooting
int getTopServoAngle() {
    return(topServoCurrentAngle);
};

int getBottomServoAngle() {
    return(bottomServoCurrentAngle);
};

bool areServosEnabled(){
    return (topServoConnected && bottomServoConnected);
};

float getCurrentServoOffset() {
    return currentServoOffset;
}
// need function to go to slack (both servos all the way in)

void servosSlack(){
    topServo.write(TOP_SERVO_SLACK_ANGLE);
    bottomServo.write(BOTTOM_SERVO_SLACK_ANGLE);
    topServoCurrentAngle = TOP_SERVO_SLACK_ANGLE;
    bottomServoCurrentAngle = BOTTOM_SERVO_SLACK_ANGLE;  
}


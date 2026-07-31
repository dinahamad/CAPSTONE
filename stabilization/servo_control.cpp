#include <Arduino.h>
#include <ESP32Servo.h>
#include "servo_control.h"

// --------- SERVO CONSTANTS
// Servo pins
const int WRIST_PITCH_PIN = 14; // up and down
//const int WRIST_YAW_PIN = #; // side to side
//const int ELBOW_PIN = #;

// Servo starting positions, based on gemoetry of arm
const int WRIST_PITCH_START = 90;
//const int WRIST_YAW_START  = 90;
//const int ELBOW_START       = 90;

// Servo soft limits
const int WRIST_PITCH_MIN = 40;
const int WRIST_PITCH_MAX = 140;
//const int WRIST_YAW_MIN = #;
//const int WRIST_YAW_MAX = #;
//const int ELBOW_MIN = #;
//const int ELBOW_MAX = #;

Servo wristPitchServo;
//Servo wristYawServo; final goal: have three servos
//Servo elbowServo;

void setupServos()
{
    wristPitchServo.attach(WRIST_PITCH_PIN);
    //wristYawServo.attach(WRIST_SIDE_PIN);
    //elbowServo.attach(ELBOW_PIN);

    // Start all servos at their centre positions
    wristPitchServo.write(WRIST_PITCH_START);
    //wristSideServo.write(WRIST_YAW_START);
    //elbowServo.write(ELBOW_START);
}

void moveWristPitch(int angle)
{
    angle = constrain(angle, WRIST_PITCH_MIN, WRIST_PITCH_MAX);
    wristPitchServo.write(angle);
}

/*
void moveWristYaw(int angle)
{
    angle = constrain(angle, WRIST_YAW_MIN, WRIST_YAW_MAX);
    wristSideServo.write(angle);
}

void moveElbow(int angle)
{
    angle = constrain(angle, ELBOW_MIN, ELBOW_MAX);
    elbowServo.write(angle);
}
*/

#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// This file is for the servos that pull the hand up and down relative to the forearm (pitch)
/////////////////////////////////////////////////////////////////////////////////////////////


// --------- SERVO CONSTANTS
// Servo pins, pins i can use: GPIO 27, 33, 32, 26
const int TOP_SERVO_PIN = 27; 
const int BOTTOM_SERVO_PIN = 33;

// Positions that wrist and forearm IMU's are parallel 
// pitch (up down) and - (side to side wrist) are zero
const int TOP_SERVO_ORIGIN_ANGLE = 90;
const int BOTTOM_SERVO_ORIGIN_ANGLE = 90;

// Servo soft safety limits
// 90 deg is middle, 30 is flexed down, 150 is flexed up
// 0-> 180 
const int TOP_SERVO_MIN_ANGLE = 30;
const int TOP_SERVO_MAX_ANGLE = 150;

const int BOTTOM_SERVO_MIN_ANGLE = 30;
const int BOTTOM_SERVO_MAX_ANGLE = 150;

// Servo Pull directions, change sign if servo pulling in wrong direction
//const int TOP_SERVO_DIRECTION = 1;     
//const int BOTTOM_SERVO_DIRECTION = 1; 

// Functions
void setupServos();

// for voluntary movement
void setWristPitchValue(float correction);
void moveHandUp(int amount);
void moveHandDown(int amount);

// for tremor movement

// move servos to origin
void originAngles();

void endProgram();

// Information for debugging
int getTopServoAngle();
int getBottomServoAngle();
bool areServosEnabled();

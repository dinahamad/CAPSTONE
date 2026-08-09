#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>
#include "pins.h"


// --------- servo pins - should add in pins.h
// Servo pins, pins i can use: GPIO 27, 33, 32, 26
const int TOP_SERVO_PIN = 27; //was 33
const int BOTTOM_SERVO_PIN = 33; //was 27 but figrued out imus were mounted wrong after rewiring

// Servo positions that wrist and forearm IMU's are parallel 
const int TOP_SERVO_ORIGIN_ANGLE = 90;
const int BOTTOM_SERVO_ORIGIN_ANGLE = 90;

const int TOP_SERVO_SLACK_ANGLE = 180;
const int BOTTOM_SERVO_SLACK_ANGLE = 180;

// Servo soft safety limits
// 90 deg is middle, 30 is flexed down, 150 is flexed up
// 0-> 180 
const int TOP_SERVO_MIN_ANGLE = 30;
const int TOP_SERVO_MAX_ANGLE = 150;

const int BOTTOM_SERVO_MIN_ANGLE = 30;
const int BOTTOM_SERVO_MAX_ANGLE = 150;

// Servo Pull directions, change sign if servo pulling in wrong direction
const int TOP_SERVO_DIRECTION = 1;      
const int BOTTOM_SERVO_DIRECTION = 1;  

const float VOLUNTARY_MOVEMENT_GAIN = 2.0f; // Tried at 3.0f
const float TREMOR_CORRECTION_GAIN = 6.0f; // to keep in the selected voluntary median
// add the gain for opposing motion
const float TREMOR_OPPOSITION_GAIN = 3.0f;

const float MAX_SERVO_OFFSET = 180.0f;
// ignore small corrections
const float CORRECTION_DEADBAND = 0.15f;

// 0 = no movement, 1 = immediate movement.
// A smaller value makes the motion smoother.
const float SERVO_SMOOTHING = 1.0f;

// Standard hobby servos are normally updated every 20 ms
const uint32_t SERVO_UPDATE_INTERVAL_MS = 20;

// Functions
void setupServos();

// move servos to origin
void originAngles();

void updateServos(float wristPitch);

// figure out how much correction is needed
//void updateServosFromTremor();
//actually move the servos
//void writeServoAngles(float correction); // not needed in main.cpp

void endProgramServos(); 

// Information for debugging
int getTopServoAngle();
int getBottomServoAngle();
bool areServosEnabled();
float getCurrentServoOffset();
void servosSlack();





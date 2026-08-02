#pragma once
#include <Arduino.h>

// --------- SERVO CONSTANTS
// Servo pins, pins i can use: GPIO 27, 33, 32, 26
const int WRIST_PITCH_TOP_PIN = 27; 
const int WRIST_PITCH_BOTTOM_PIN = 33;

// Servo neutral positions, based on gemoetry of arm
const int WRIST_PITCH_TOP_START = 90;
const int WRIST_PITCH_BOTTOM_START = 90;

// Servo soft safety limits
const int WRIST_PITCH_TOP_MIN = 40;
const int WRIST_PITCH_TOP_MAX = 140;

const int WRIST_PITCH_BOTTOM_MIN = 40;
const int WRIST_PITCH_BOTTOM_MAX = 140;

// Servo Pull directions
// Change sign if a servo turns the wrong way
const int WRIST_PITCH_TOP_DIRECTION = 1;
const int WRIST_PITCH_BOTTOM_DIRECTION = 1;

void setupServos();
void setWristCorrection(float correction);
void moveHandUp(int amount);
void moveHandDown(int amount);
void stopWrist();

// Information for debugging/UI
int getWristPitchTopServoCommand();
int getWristPitchBottomServoCommand();
bool areServosEnabled();

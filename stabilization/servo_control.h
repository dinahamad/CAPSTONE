#pragma once
#include <Arduino.h>

// --------- SERVO CONSTANTS
// Servo pins
const int WRIST_PITCH_TOP_PIN = 14; 
const int WRIST_PITCH_BOTTOM_PIN = 15;

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

// Maximum amount each servo can move away from neutral
const int WRIST_PITCH_TOP_PULL_RANGE = 35;
const int WRIST_PITCH_BOTTOM_PULL_RANGE = 35;

void setupServos();
void moveHandUp(int amount);
void moveHandDown(int amount);
void stopWrist();

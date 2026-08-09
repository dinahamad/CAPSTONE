// #include <Arduino.h>
// #include <ESP32Servo.h>
// #include "servo.h"
// #include <math.h>
// #include "tremor_detection.h"

// /////////////////////////////////////////////////////////////////////////////////////////////
// // This file is for the servos that pull the hand up and down relative to the forearm (pitch)
// /////////////////////////////////////////////////////////////////////////////////////////////

// // Servo objects
// Servo topServo;
// Servo bottomServo;

// // flags for servo connections
// bool topServoConnected = false;
// bool bottomServoConnected = false;

// // start at neutral, changes for evreythime it moves
// int topServoCurrentAngle = TOP_SERVO_ORIGIN_ANGLE;
// int bottomServoCurrentAngle = BOTTOM_SERVO_ORIGIN_ANGLE;

// static float currentServoOffset = 0.0f;
// static uint32_t previousServoUpdateTime = 0;

// // takes one wrist offset and converts it into the two opposite servo angles
// // realtive to center
// static void writeServoAngles(float servoOffset)
// {
//     int topTargetAngle = (int)roundf(TOP_SERVO_ORIGIN_ANGLE + TOP_SERVO_DIRECTION * servoOffset);
//     int bottomTargetAngle = (int)roundf(BOTTOM_SERVO_ORIGIN_ANGLE - BOTTOM_SERVO_DIRECTION * servoOffset);

//     topTargetAngle = constrain(topTargetAngle,TOP_SERVO_MIN_ANGLE,TOP_SERVO_MAX_ANGLE);

//     bottomTargetAngle = constrain(bottomTargetAngle,BOTTOM_SERVO_MIN_ANGLE,BOTTOM_SERVO_MAX_ANGLE);

//     if (topTargetAngle != topServoCurrentAngle) {
//         topServo.write(topTargetAngle);
//         topServoCurrentAngle = topTargetAngle;
//     }

//     if (bottomTargetAngle != bottomServoCurrentAngle){
//         bottomServo.write(bottomTargetAngle);
//         bottomServoCurrentAngle = bottomTargetAngle;
//     }
// }

// // gets its info directly from tremorDetected() and getFilteredPitch()
// void updateServos(float wristPitch){
//     if (!areServosEnabled()){return;}

//     // current time of esp32
//     uint32_t currentTime = millis();

//     // to esnure servos need 20ms time between reciving signals
//     if (currentTime - previousServoUpdateTime< SERVO_UPDATE_INTERVAL_MS){
//         return;
//     }

//     previousServoUpdateTime = currentTime;

//     // The full measured wrist motion.
//     float measuredMovement = VOLUNTARY_MOVEMENT_GAIN * wristPitch;

//     // The detected tremor part of that movement.
//     float tremorMovement = TREMOR_CORRECTION_GAIN * getFilteredPitch();

//     // Follow voluntary movement while removing tremor.
//     float targetServoOffset = measuredMovement - tremorMovement;

//     // Prevent excessive servo movement.
//     targetServoOffset = constrain(targetServoOffset,-MAX_SERVO_OFFSET,MAX_SERVO_OFFSET);

//     // Gradually approach the target instead of jumping.
//     currentServoOffset += SERVO_SMOOTHING * (targetServoOffset - currentServoOffset);

//     writeServoAngles(currentServoOffset);
// }

// // connect servos to microcontroller
// void setupServos() {
//     // Connect each servo object to its pin
//     topServo.attach(TOP_SERVO_PIN);
//     bottomServo.attach(BOTTOM_SERVO_PIN);

//     // Put both servos at their starting positions
//     topServoConnected = true;
//     bottomServoConnected = true;
//     //originAngles();
// }

// // put servos to their origin
// void originAngles() {
//     currentServoOffset = 0.0f;
//     topServo.write(TOP_SERVO_ORIGIN_ANGLE);
//     bottomServo.write(BOTTOM_SERVO_ORIGIN_ANGLE);

//     // update current angles
//     topServoCurrentAngle = TOP_SERVO_ORIGIN_ANGLE;
//     bottomServoCurrentAngle = BOTTOM_SERVO_ORIGIN_ANGLE;   
// }

// void endProgramServos() {
//     topServo.detach();
//     bottomServo.detach();
//     topServoConnected = false;
//     bottomServoConnected = false;
// }

// // for torubleshooting
// int getTopServoAngle() {
//     return(topServoCurrentAngle);
// };

// int getBottomServoAngle() {
//     return(bottomServoCurrentAngle);
// };

// bool areServosEnabled(){
//     return (topServoConnected && bottomServoConnected);
// };

// float getCurrentServoOffset() {
//     return currentServoOffset;
// }
// // need function to go to slack (both servos all the way in)

// void servosSlack(){
//     topServo.write(TOP_SERVO_SLACK_ANGLE);
//     bottomServo.write(BOTTOM_SERVO_SLACK_ANGLE);
//     topServoCurrentAngle = TOP_SERVO_SLACK_ANGLE;
//     bottomServoCurrentAngle = BOTTOM_SERVO_SLACK_ANGLE;  
// }


//#include "globals.h"
//#include "servo_control.h"

//#pragma once

//bool isHandIMUConnected();
//bool isForearmIMUConnected();

// useful variables from main
// float getRelativeOrientation{return relativeOrientation;} // should be a degree value btween 0 -> 180deg

// Troubleshooting
//bool isHandIMUConnected(){return imuConnected[0];}
//bool isForearmIMUConnected(){return imuConnected[1];}
//bool isServosConnected(){return servosConnected[0] && servosConnected[1];}




// ---------------------------- DELETE EVERYTHING PAST THIS LINE
//const int HAND_IMU = 0;
//const int FOREARM_IMU = 1;

//const int ROLL = 0;
//const int PITCH = 1;
//const int YAW = 2;

// of Hand IMU
// float rollAngleHand() {return orientation[HAND_IMU][ROLL];} // units: degrees
// float pitchAngleHand(){return orientation[HAND_IMU][PITCH];}
// float yawAngleHand(){return orientation[HAND_IMU][YAW];}

// of forearm IMU
//float forearmRoll(){return orientation[FOREARM_IMU][ROLL];}
//float forearmPitch(){return orientation[FOREARM_IMU][PITCH];}
//float forearmYaw(){return orientation[FOREARM_IMU][YAW];}

// angle relative difference between hand and forearm - all basic uses this
// float wristRoll(){return relativeOrientation[ROLL];}
// float wristPitch(){return relativeOrientation[PITCH];}
// float wristYaw(){return relativeOrientation[YAW];}

// between hand and forearm
// float gettremorFrequencyHz(){return tremorFrequencyHz;}
// float batteryPercentage(); // disconnect servos once reaches a certain battery


// Troubleshooting
// bool isHandIMUConnected(){return imuConnected[HAND_IMU_INDEX];
// bool isForearmIMUConnected(){return imuConnected[FOREARM_IMU_INDEX];}

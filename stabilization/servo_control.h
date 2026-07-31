// this file

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

void setupServos();
void moveWristPitch();

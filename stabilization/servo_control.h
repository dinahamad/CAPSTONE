// this file

// --------- SERVO CONSTANTS
// Servo pins
const int WRIST_PITCH_TOP_PIN = 14; 
const int WRIST_PITCH_BOTTOM_PIN = 15;

// Servo starting positions, based on gemoetry of arm
const int WRIST_PITCH_TOP_START = 90;
const int WRIST_PITCH_BOTTOM_START = 90;

// Servo soft limits
const int WRIST_PITCH_MIN = 40;
const int WRIST_PITCH_MAX = 140;

void setupServos();
void moveWristTopPitch(int angle);
void moveWristBottomPitch(int angle);

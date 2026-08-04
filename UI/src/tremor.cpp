// #include <Arduino.h>
// #include <SPI.h>
// #include "ICM_20948.h"
// #include <ESP32Servo.h>
// #include <math.h>


// // ============================================================
// // PIN CONNECTIONS, IMU AND SERVO OBJECTS


// #define SCK_PIN    5
// #define MISO_PIN   21
// #define MOSI_PIN   19
// #define CS_HAND    27
// #define SERVO_PIN  14
// ICM_20948_SPI imu;
// Servo correctionServo;


// // ============================================================//============================================================
// // GLOBAL CONSTANT DEFINITIONS




// // Soft on/off switch for the motor
// const bool ENABLE_SERVO_OUTPUT = true;






// // IMU Flat measurment calibration for deadband values
// // for differetn IMU, do program #1 (basic IMU calibration) to replace values
// const float FLAT_MIN_RAW_ANGLE = -1.855;
// const float FLAT_MAX_RAW_ANGLE =  0.585;
// const float FLAT_CENTER_ANGLE  = -0.685;








// // Speeds below 1deg/sec are considered zero
// // Increase if actuator changes while IMU is motionless
// const float SPEED_DEADBAND_DPS = 1.0;


// // Change to -1.0 if gyroscope velocity signs are oppositite
// const float GYRO_DIRECTION = 1.0;


// // For gyroscope callibration, use 300 samples at beginning
// const int GYRO_CALIBRATION_SAMPLES = 300;


// // Measured automatically during setup.
// float gyroOffsetY = 0.0;








// // Filtered Angle
// // accelerometer gives weak angle signal during fast motion
// // gyroscope can read angles during fast movements better but drifts for constant angles
// // instead, use both sensors
// // In this case larger value "trusts" gyro more. range: 0.2 - 1.0s
// const float FILTER_TIME_CONSTANT_SECONDS = 0.5;


// // Current (filtered) absolute angle.
// float currentAngle = 0.0;


// // Time of the previous IMU reading.
// uint32_t previousSensorTimeMicros = 0;










// // Maximum speed for IMU that is considered stationary, deg/sec
// const float STILL_SPEED_DPS = 10.0;


// // Time the hand must remain still to be considered stationary, ms
// const uint32_t STILL_TIME_MS = 350;


// // Maximum allowed angle change while checking for stillness.
// const float STILL_MAX_ANGLE_CHANGE_DEG = 0.8;


// // Current stationary position around which oscillation is measured.
// float centerAngle = 0.0;


// // Variables used while checking whether the hand is still.
// bool checkingForStillPosition = false;
// uint32_t stillStartTime = 0;
// float stillStartAngle = 0.0;










// // tremors go up to maximum 10deg from center position
// const float MAX_OSCILLATION_ANGLE_DEG = 3.0;


// // Ignore very small oscillations, 0.5deg
// const float MIN_OSCILLATION_ANGLE_DEG = 0.5;


// // tremor velocity must be at least 70deg/sec
// const float MIN_OSCILLATION_SPEED_DPS = 10.0;


// // A reversal direction speeed less than this is ignored so it doesnt confuse actual oscillations
// const float DIRECTION_THRESHOLD_DPS = 5.0;


// // # of oscillations before program regognizes tremor and activates actuator.
// // Increase if false detections occur
// const int REQUIRED_REVERSALS = 2;


// // In this mode, when true, the program does not wait for at least half a period of a tremor to start correcting. Instead AS SOON AS the
// // 10deg movement in at least 70deg/sec movement happens, the actuators actuate to reverse the motion
// const bool EARLY_ASSIST_ENABLED = false;


// // Keep the early assistance on for this long after the 10deg/70deg.sec movement happens, letting it continue when speed = 0 at top of oscillation
// const uint32_t FAST_MOTION_HOLD_MS = 250;


// // Direction reversal at peak of oscialltion needs to happen in max 400ms or the movement not treated as the same oscillation
// const uint32_t REVERSAL_MEMORY_MS = 400;


// // Stop calling the motion an oscillation if no movement that is 10deg and 70deg/sec has happened in this long
// const uint32_t OSCILLATION_TIMEOUT_MS = 600;


// // if movement goes beyond 10+2deg, assume it a large movement aka voluntary motion
// const float OSCILLATION_ABORT_MARGIN_DEG = 2.0;


// // Oscillation detection variables.
// bool oscillationDetected = false;


// int previousDirection = 0;
// int reversalCount = 0;


// uint32_t lastFastMotionTime = 0;










// // Servo position when no correction is applied
// const float SERVO_NEUTRAL_DEG = 90.0;


// // change to 1.0 if direction of actuaotor is backward
// const float SERVO_DIRECTION = -1.0;


// // Gain = 1.0: 10deg hand motion==10deg servo motion.
// // Gain = 0.5: 10deg hand motion==5deg servo motion.
// // Gain = 1.5: 10deg hand motion==15 degrees servo motion.
// const float SERVO_GAIN_POSITIVE_ANGLE = 5.0;
// const float SERVO_GAIN_NEGATIVE_ANGLE = 5.0;


// // Servo max angle soft stop
// const float SERVO_MAX_CORRECTION_DEG = 60.0;


// // Absolute servo limits (center is 90deg)
// const float SERVO_MIN_DEG = 30.0;
// const float SERVO_MAX_DEG = 160.0;


// // DELETE
// const int SERVO_MIN_PULSE_US = 1000;
// const int SERVO_MAX_PULSE_US = 2000;


// // Minimum time between servo commands.
// const uint32_t SERVO_UPDATE_INTERVAL_MS = 10;


// uint32_t lastServoUpdateTime = 0;


// // Current angle target calculated for the servo.
// float servoTargetAngle = SERVO_NEUTRAL_DEG;








// // how many ms between each serial moniter message. pritning too much can slow program
// const uint32_t SERIAL_PRINT_INTERVAL_MS = 500;
// uint32_t lastSerialPrintTime = 0;


// // ============================================================// ============================================================
// // HELPER FUNCTIONS




// // 1. Ensure an inputted value stays within the bounds of some range
// //      If not already in range, bring to one of the bounds


// float limitValue(float value, float minimumValue, float maximumValue)
// {
//     if (value < minimumValue) {
//         return minimumValue;
//     }


//     if (value > maximumValue) {
//         return maximumValue;
//     }
//     return value;
// }




// // 2. Read current acceleromter angle in rad
// float readAccelerometerAngle() {
//     // X-axis: Along the length of the hand / seesaw.
//     // Y-axis: Along the rotation axis.
//     // Z-axis:Normal to top of the hand.
//     float ax = imu.accX();
//     float ay = imu.accY();
//     float az = imu.accZ();
//     float angleRadians = atan2(-ax, sqrt(ay * ay + az * az));
//     return angleRadians * 180.0 / PI;
// }




// // 3. Check if IMu is flat/parallel to floor (aka within deadband of being parallel to floor)
// bool isFlat(float rawAngle){
//     return (rawAngle >= FLAT_MIN_RAW_ANGLE && rawAngle <= FLAT_MAX_RAW_ANGLE);
// }




// // 4. Connect to IMU and show error if not connected
// void connectToIMU() {
//     while (true) {
//         imu.begin(CS_HAND, SPI);


//         if (imu.status == ICM_20948_Stat_Ok) {
//             Serial.println("IMU connected.");
//             return;
//         }


//         Serial.print("IMU connection failed: ");
//         Serial.println(imu.statusString());


//         Serial.println("Trying again...");
//         delay(2000);
//     }
// }




// // 5. Calibrate gyroscope speed measurements (used when IMU powered on)
// void calibrateGyroscope() {
//     Serial.println();
//     Serial.println("Calibrating gyroscope.");
//     Serial.println("NOTE: Keep the IMU completely still.");


//     float totalSpeed = 0.0;
//     int samplesCollected = 0;


//     while (samplesCollected < GYRO_CALIBRATION_SAMPLES) {
//         if (imu.dataReady()) {
//             imu.getAGMT();
//             totalSpeed += imu.gyrY();
//             samplesCollected++;
//             delay(3);
//         }
//     }


//     gyroOffsetY = totalSpeed / GYRO_CALIBRATION_SAMPLES;


//     Serial.print("Gyroscope offset: ");
//     Serial.print(gyroOffsetY, 3);
//     Serial.println(" deg/s");


//     Serial.println("Gyroscope calibration complete.");
//     Serial.println();
// }




// // 6. measure angle of gyro upon sdtartup
// void initializeAngle() {
//     Serial.println("Measuring initial position.");
//     Serial.println("Keep the IMU still.");


//     // use the avg of 50 samples
//     const int INITIAL_ANGLE_SAMPLES = 50;


//     float totalAngle = 0.0;
//     int samplesCollected = 0;


//     while (samplesCollected < INITIAL_ANGLE_SAMPLES) {
//         if (imu.dataReady()) {
//             imu.getAGMT();


//             float rawAngle = readAccelerometerAngle();


//             // Correct the flat-position offset.
//             float correctedAngle = rawAngle - FLAT_CENTER_ANGLE;


//             totalAngle += correctedAngle;
//             samplesCollected++;


//             delay(3);
//         }
//     }
//     currentAngle = totalAngle / INITIAL_ANGLE_SAMPLES;


//     // center aka where oscillations can occur by
//     centerAngle = currentAngle;


//     previousSensorTimeMicros = micros();


//     Serial.print("Initial angle: ");
//     Serial.print(currentAngle, 2);
//     Serial.println(" deg");


//     Serial.print("Initial center: ");
//     Serial.print(centerAngle, 2);
//     Serial.println(" deg");


//     Serial.println();
// }




// // 7. Determine movement direction (down -> -1, up -> 1)
// int getMovementDirection(float angularSpeed) {
//     if (angularSpeed > DIRECTION_THRESHOLD_DPS) {
//         return 1;
//     }


//     if (angularSpeed < -DIRECTION_THRESHOLD_DPS) {
//         return -1;
//     }


//     return 0;
// }




// // 8. Reset variables once tremor oscillation ends
// void resetOscillationDetection() {
//     oscillationDetected = false;
//     previousDirection = 0;
//     reversalCount = 0;
//     lastFastMotionTime = 0;
// }




// // 9. update oscillation detection
// void updateOscillationDetection(float angleFromCenter, float angularSpeed, uint32_t currentTime) {
//     float absoluteAngle = fabs(angleFromCenter);
//     float absoluteSpeed = fabs(angularSpeed);


//     // cancel detection if movement too large
//     float maximumAllowedAngle = MAX_OSCILLATION_ANGLE_DEG + OSCILLATION_ABORT_MARGIN_DEG;


//     // if angle larger than 10deg+2deg
//     if (absoluteAngle > maximumAllowedAngle) {
//         resetOscillationDetection();
//         return;
//     }


//     // check for qualifying tremor motion
//     bool fastMotionNow =
//         absoluteAngle >= MIN_OSCILLATION_ANGLE_DEG &&
//         absoluteAngle <= MAX_OSCILLATION_ANGLE_DEG && // >10deg, <-10deg, and
//         absoluteSpeed >= MIN_OSCILLATION_SPEED_DPS; // faster than 70deg/s


//     if (fastMotionNow) {
//         lastFastMotionTime = currentTime;
//     }


//     // check for a direction reversal aka oscilaltion
//     int currentDirection = getMovementDirection(angularSpeed);


//     // direction is zero ~ speed is 0
//     // program keeps the previous nonzero direction
//     if (currentDirection != 0) {
//         if (previousDirection == 0) {
//             previousDirection = currentDirection;
//         }
//         else if (currentDirection != previousDirection) {
//             // one count reversal if the "tremor/fast" motion happened recently
//             bool fastMotionWasRecent = lastFastMotionTime != 0 && currentTime - lastFastMotionTime <= REVERSAL_MEMORY_MS;


//             if (fastMotionWasRecent) {
//                 reversalCount++;
//                 if (reversalCount >= REQUIRED_REVERSALS) {
//                     oscillationDetected = true;
//                 }
//             }
//             else {
//                 reversalCount = 0;
//                 oscillationDetected = false;
//             }
//             previousDirection = currentDirection;
//         }
//     }


//     // stop tremor detection once oscialliton edns
//     if (lastFastMotionTime != 0 && currentTime - lastFastMotionTime > OSCILLATION_TIMEOUT_MS) {
//         resetOscillationDetection();
//     }
// }


// // 10. Check whether servo should be on and working
// bool shouldAssist(uint32_t currentTime) {
//     // Once oscialltion is confirmed by reversal, actuator assistance stays on until oscialltion is cancelled
//     if (oscillationDetected) {
//         return true;
//     }


//     // actuatr starts immedietly after tremor motion is detected, remaining active briefly so it doesnt utrn off at top and bottom
//     // of each oscialltion
//     bool recentFastMotion = lastFastMotionTime != 0 && currentTime - lastFastMotionTime <= FAST_MOTION_HOLD_MS;


//     if (EARLY_ASSIST_ENABLED && recentFastMotion) {
//         return true;
//     }
//     return false;
// }




// // 11. Update the "STILL" position center
// // Update the center whenever the IMU remains still long enough
// void updateCenterPosition(float measuredAngle,float angularSpeed,uint32_t currentTime) {
//     // If speed is too high, theres tremor.
//     // Restart the still-position timer.
//     if (fabs(angularSpeed)>STILL_SPEED_DPS) {
//         checkingForStillPosition=false;
//         return;
//     }


//     // The hand has just become still. Begin timing the stationary period.
//     if (!checkingForStillPosition) {
//         checkingForStillPosition=true;
//         stillStartTime=currentTime;
//         return;
//     }


//     // The hand has remained still long enough.
//     if (currentTime-stillStartTime>=STILL_TIME_MS) {
//         centerAngle=measuredAngle;
//         checkingForStillPosition=false;


//         // Clear information from the previous center.
//         resetOscillationDetection();


//         Serial.print("NEW CENTER: ");
//         Serial.print(centerAngle,2);
//         Serial.println(" deg");
//     }
// }




// // 12. Calculate the servo target angle
// float calculateServoTarget(float angleFromCenter)
// {
//     // select gain based on movement direction (- or +)
//     float selectedGain;
//     if (angleFromCenter >= 0.0) {
//         selectedGain = SERVO_GAIN_POSITIVE_ANGLE;
//     }
//     else {
//         selectedGain = SERVO_GAIN_NEGATIVE_ANGLE;
//     }


//     // calcualte the opposite servo angle movement it should use for correction
//     float correction = SERVO_DIRECTION * selectedGain * angleFromCenter;


//     // Limit the correction around neutral (softstop)
//     correction = limitValue(correction, -SERVO_MAX_CORRECTION_DEG, SERVO_MAX_CORRECTION_DEG);
//     float target = SERVO_NEUTRAL_DEG + correction;


//     // Apply the final servo limits.
//     return limitValue(target, SERVO_MIN_DEG,SERVO_MAX_DEG);
// }




// // 13. Get the state of motion to send to the serial moniter
// const char *getMotionState(bool flatPosition, float angularSpeed, bool assistanceActive) {
//     if (oscillationDetected) {
//         return "OSCILLATION";
//     }


//     if (assistanceActive) {
//         return "FAST_MOVEMENT";
//     }


//     if (fabs(angularSpeed) <= STILL_SPEED_DPS) {
//         if (flatPosition) {
//             return "EQUILIBRIUM";
//         }
//         return "STILL";
//     }
//     return "NORMAL_MOVEMENT";
// }




// // ============================================================
// // SETUP


// void setup()
// {
//     Serial.begin(9600);
//     delay(2000);
//     Serial.println();
//     Serial.println("Starting IMU system.");
//     // Start SPI communication.
//     SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN);
//     pinMode(CS_HAND, OUTPUT);
//     digitalWrite(CS_HAND, HIGH);
//     // Connect to the IMU.
//     connectToIMU();


//     // Start the servo only when servo output is enabled (softstop variable)
//     if (ENABLE_SERVO_OUTPUT) {
//         correctionServo.attach(SERVO_PIN, SERVO_MIN_PULSE_US, SERVO_MAX_PULSE_US
//         );


//         correctionServo.write((int)SERVO_NEUTRAL_DEG); // move to servos neutral position in brace
//         delay(500);
//     }


//     // Calibrate speed and measure the initial position.
//     calibrateGyroscope();
//     initializeAngle();


//     Serial.println("System ready.");


//     if (ENABLE_SERVO_OUTPUT) {
//         Serial.println("Servo output: ENABLED");
//     }
//     else {
//         Serial.println("Servo output: DISABLED");
//     }
//     Serial.println();
// }




// // ============================================================
// // MAIN LOOP


// void loop()
// {
//     // Wait until the IMU has a new measurement.
//     if (!imu.dataReady()) {
//         return;
//     }
//     imu.getAGMT();


//     // Calulate time since previous reading
//     uint32_t currentTimeMicros = micros();
//     float elapsedSeconds = (currentTimeMicros - previousSensorTimeMicros)/ 1000000.0;
//     previousSensorTimeMicros = currentTimeMicros;


//     // prevent large jumps by preventing long delays in the system to be used in oscialltion calcaultion if prgram delays
//     if (elapsedSeconds <= 0.0 ||elapsedSeconds > 0.1) {
//         elapsedSeconds = 0.01;
//     }


//     // number of ms since the program started running
//     uint32_t currentTime = millis();


//     // read accelerometer angle
//     float rawAccelerometerAngle = readAccelerometerAngle();
//     float accelerometerAngle = rawAccelerometerAngle -FLAT_CENTER_ANGLE; // center on deadband center


//     // read gyroscope speed
//     float angularSpeed =(imu.gyrY() - gyroOffsetY) * GYRO_DIRECTION;


//     // calculate how much the angle filter should trust the gyroscope angle compared with the accelerometer angle
//     float filterAmount = FILTER_TIME_CONSTANT_SECONDS / (FILTER_TIME_CONSTANT_SECONDS + elapsedSeconds);


//     // first use gyrroscope speed to predict the new angle
//     // then use the accelerometer angle to slowly correct
//     currentAngle = filterAmount *
//         (currentAngle + angularSpeed * elapsedSeconds) +
//         (1.0 - filterAmount) *
//         accelerometerAngle;


//     // apply speed deadband
//     if (fabs(angularSpeed) < SPEED_DEADBAND_DPS) {
//         angularSpeed = 0.0;
//     }


//     // First check whether the hand has stopped at a new center
//     updateCenterPosition(currentAngle,angularSpeed,currentTime);


//     // Calculate movement relative to the newest center
//     float angleFromCenter=currentAngle-centerAngle;


//     // Detect oscillation around that center
//     updateOscillationDetection(angleFromCenter,angularSpeed,currentTime);




//     // Decide whether the servo should assist
//     bool assistanceActive=shouldAssist(currentTime);


//     // calcualte and send servo command
//     if (assistanceActive) {
//         servoTargetAngle = calculateServoTarget(angleFromCenter);
//     }
//     else {
//         servoTargetAngle = SERVO_NEUTRAL_DEG;
//     }


//     if (ENABLE_SERVO_OUTPUT && currentTime - lastServoUpdateTime >= SERVO_UPDATE_INTERVAL_MS) {
//         correctionServo.write((int)round(servoTargetAngle));
//         lastServoUpdateTime = currentTime;
//     }


//     // print test info
//     if (currentTime - lastSerialPrintTime >= SERIAL_PRINT_INTERVAL_MS) {
//         bool flatPosition = isFlat(rawAccelerometerAngle);
//         float displayedAngle = currentAngle;


//         // display exactly zero when IMU inside deadband range and not moving
//         if (flatPosition && fabs(angularSpeed) <= STILL_SPEED_DPS) {
//             displayedAngle = 0.0;
//         }


//         Serial.print("Angle: ");
//         Serial.print(displayedAngle, 2);
//         Serial.print(" deg");


//         Serial.print("    Center: ");
//         Serial.print(centerAngle, 2);
//         Serial.print(" deg");


//         Serial.print("    From center: ");
//         Serial.print(angleFromCenter, 2);
//         Serial.print(" deg");


//         Serial.print("    Speed: ");
//         Serial.print(angularSpeed, 2);
//         Serial.print(" deg/s");


//         Serial.print("    Reversals: ");
//         Serial.print(reversalCount);


//         Serial.print("    State: ");
//         Serial.print(getMotionState(flatPosition, angularSpeed, assistanceActive));


//         Serial.print("    Servo target: ");
//         Serial.print(servoTargetAngle, 1);
//         Serial.print(" deg");


//         Serial.print("    Servo output: ");
//         if (ENABLE_SERVO_OUTPUT && assistanceActive) {
//             Serial.println("ACTIVE");
//         }
//         else if (!ENABLE_SERVO_OUTPUT && assistanceActive) {
//             Serial.println("DETECTED_BUT_DISABLED");
//         }
//         else {
//             Serial.println("OFF");
//         }


//         lastSerialPrintTime = currentTime;
//     }
// }

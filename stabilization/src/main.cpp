// Didi Code

#include <Arduino.h>
#include <SPI.h>
#include "ICM_20948.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "pins.h"
#include "globals.h"
#include "servo_control.h"


// ============================================================
// FUNCTION DECLARATIONS
// ============================================================

void calibrateIMUs();
void startSystem();
void stopSystem();
void resetOrientation();
void calculateAccelAngles(
    float ax,
    float ay,
    float az,
    float &roll,
    float &pitch
);

void updateOrientation(
    int i,
    float ax,
    float ay,
    float az,
    float gx,
    float gy,
    float gz,
    uint64_t timestamp_us
);

void calculateRelativeOrientation();
void sendIMUData(
    int i,
    uint64_t timestamp_us
);

void sendStatus();
void readIMUs();

// ============================================================
// BLE SERVER CALLBACKS
// ============================================================

class ServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer* server)
    {
        bleConnected = true;

        Serial.println("BLE CLIENT CONNECTED");

        sendStatus();
    }

    void onDisconnect(BLEServer* server)
    {
        bleConnected = false;

        Serial.println("BLE CLIENT DISCONNECTED");

        BLEDevice::startAdvertising();
    }
};

// ============================================================
// BLE COMMAND CALLBACKS
// ============================================================

class CommandCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic* characteristic)
    {
        std::string value =
            characteristic->getValue();

        if (value.length() == 0)
        {
            return;
        }

        String command =
            String(value.c_str());

        command.trim();
        command.toUpperCase();

        Serial.print("BLE COMMAND: ");
        Serial.println(command);

        // ====================================================
        // START
        // ====================================================

        if (command == "START")
        {
            startSystem();
        }

        // ====================================================
        // STOP
        // ====================================================

        else if (command == "STOP")
        {
            stopSystem();
        }

        // ====================================================
        // CALIBRATE
        // ====================================================

        else if (command == "CALIBRATE")
        {
            calibrateIMUs();
        }

        // ====================================================
        // STATUS
        // ====================================================

        else if (command == "STATUS")
        {
            sendStatus();
        }

        // ====================================================
        // RESET
        // ====================================================

        else if (command == "RESET")
        {
            resetOrientation();

            Serial.println(
                "ORIENTATION RESET"
            );
        }
    }
};

// ============================================================
// CALCULATE ACCELEROMETER ROLL AND PITCH
// ============================================================

void calculateAccelAngles(
    float ax,
    float ay,
    float az,
    float &roll,
    float &pitch
)
{
    roll =
        atan2(
            ay,
            az
        )
        * 180.0f
        / PI;

    pitch =
        atan2(
            -ax,
            sqrt(
                ay * ay +
                az * az
            )
        )
        * 180.0f
        / PI;
}

// ============================================================
// UPDATE ORIENTATION
// ============================================================

void updateOrientation(
    int i,
    float ax,
    float ay,
    float az,
    float gx,
    float gy,
    float gz,
    uint64_t timestamp_us
)
{
    if (lastTimestamp_us[i] == 0)
    {
        float rollAccel;
        float pitchAccel;

        calculateAccelAngles(
            ax,
            ay,
            az,
            rollAccel,
            pitchAccel
        );

        orientation[i][0] =
            rollAccel;

        orientation[i][1] =
            pitchAccel;

        orientation[i][2] =
            0.0f;

        lastTimestamp_us[i] =
            timestamp_us;

        return;
    }

    float dt =
        (
            timestamp_us -
            lastTimestamp_us[i]
        )
        /
        1000000.0f;

    lastTimestamp_us[i] =
        timestamp_us;

    if (
        dt <= 0.0f ||
        dt > 0.1f
    )
    {
        return;
    }

    // Apply gyroscope bias correction

    gx -= gyroBias[i][0];
    gy -= gyroBias[i][1];
    gz -= gyroBias[i][2];

    float roll =
        orientation[i][0];

    float pitch =
        orientation[i][1];

    float yaw =
        orientation[i][2];

    // Gyroscope integration

    float rollGyro =
        roll +
        gx * dt;

    float pitchGyro =
        pitch +
        gy * dt;

    float yawGyro =
        yaw +
        gz * dt;

    // Accelerometer correction

    float rollAccel;
    float pitchAccel;

    calculateAccelAngles(
        ax,
        ay,
        az,
        rollAccel,
        pitchAccel
    );

    // Complementary filter

    roll =
        ALPHA * rollGyro +
        (1.0f - ALPHA) * rollAccel;

    pitch =
        ALPHA * pitchGyro +
        (1.0f - ALPHA) * pitchAccel;

    yaw =
        yawGyro;

    orientation[i][0] =
        roll;

    orientation[i][1] =
        pitch;

    orientation[i][2] =
        yaw;
}

// ============================================================
// RESET ORIENTATION
// ============================================================

void resetOrientation()
{
    for (int i = 0; i < NUM_IMUS; i++)
    {
        orientation[i][0] =
            0.0f;

        orientation[i][1] =
            0.0f;

        orientation[i][2] =
            0.0f;

        lastTimestamp_us[i] =
            0;
    }

    relativeOrientation[0] =
        0.0f;

    relativeOrientation[1] =
        0.0f;

    relativeOrientation[2] =
        0.0f;
}

// ============================================================
// CALCULATE RELATIVE WRIST ORIENTATION
//
// HAND relative to FOREARM
// ============================================================

void calculateRelativeOrientation()
{
    if (
        !imuConnected[0] ||
        !imuConnected[1]
    )
    {
        return;
    }

    for (int axis = 0; axis < 3; axis++)
    {
        relativeOrientation[axis] =
            orientation[0][axis] -
            orientation[1][axis] -
            initialRelativeOrientation[axis];
    }
}

// ============================================================
// CALIBRATE IMUs
//
// The user should keep the hand and forearm still
// in the desired neutral wrist position.
//
// 1. Collect gyroscope samples
// 2. Calculate gyro bias
// 3. Capture initial relative orientation
// ============================================================

void calibrateIMUs()
{
    Serial.println();
    Serial.println(
        "=============================="
    );

    Serial.println(
        "STARTING IMU CALIBRATION"
    );

    Serial.println(
        "KEEP HAND AND FOREARM STILL"
    );

    Serial.println(
        "=============================="
    );

    systemRunning =
        false;

    calibrationComplete =
        false;

    resetOrientation();

    const int calibrationSamples =
        500;

    float gyroSum[NUM_IMUS][3] = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f}
    };

    int validSamples[NUM_IMUS] = {
        0,
        0
    };

    // ========================================================
    // COLLECT GYROSCOPE BIAS
    // ========================================================

    for (
        int sample = 0;
        sample < calibrationSamples;
        sample++
    )
    {
        for (
            int i = 0;
            i < NUM_IMUS;
            i++
        )
        {
            if (!imuConnected[i])
            {
                continue;
            }

            if (imu[i].dataReady())
            {
                imu[i].getAGMT();

                gyroSum[i][0] +=
                    imu[i].gyrX();

                gyroSum[i][1] +=
                    imu[i].gyrY();

                gyroSum[i][2] +=
                    imu[i].gyrZ();

                validSamples[i]++;
            }
        }

        delay(5);
    }

    // ========================================================
    // CALCULATE BIAS
    // ========================================================

    for (
        int i = 0;
        i < NUM_IMUS;
        i++
    )
    {
        if (
            !imuConnected[i] ||
            validSamples[i] == 0
        )
        {
            continue;
        }

        gyroBias[i][0] =
            gyroSum[i][0] /
            validSamples[i];

        gyroBias[i][1] =
            gyroSum[i][1] /
            validSamples[i];

        gyroBias[i][2] =
            gyroSum[i][2] /
            validSamples[i];

        Serial.print(
            IMU_NAMES[i]
        );

        Serial.println(
            " GYRO BIAS:"
        );

        Serial.print(
            "GX = "
        );

        Serial.println(
            gyroBias[i][0],
            4
        );

        Serial.print(
            "GY = "
        );

        Serial.println(
            gyroBias[i][1],
            4
        );

        Serial.print(
            "GZ = "
        );

        Serial.println(
            gyroBias[i][2],
            4
        );
    }

    // ========================================================
    // INITIALIZE ORIENTATION
    // ========================================================

    for (
        int i = 0;
        i < NUM_IMUS;
        i++
    )
    {
        if (!imuConnected[i])
        {
            continue;
        }

        if (imu[i].dataReady())
        {
            imu[i].getAGMT();

            float ax =
                imu[i].accX();

            float ay =
                imu[i].accY();

            float az =
                imu[i].accZ();

            float roll;
            float pitch;

            calculateAccelAngles(
                ax,
                ay,
                az,
                roll,
                pitch
            );

            orientation[i][0] =
                roll;

            orientation[i][1] =
                pitch;

            orientation[i][2] =
                0.0f;
        }
    }

    // ========================================================
    // CAPTURE INITIAL RELATIVE ORIENTATION
    //
    // Hand relative to forearm
    // ========================================================

    if (
        imuConnected[0] &&
        imuConnected[1]
    )
    {
        initialRelativeOrientation[0] =
            orientation[0][0] -
            orientation[1][0];

        initialRelativeOrientation[1] =
            orientation[0][1] -
            orientation[1][1];

        initialRelativeOrientation[2] =
            orientation[0][2] -
            orientation[1][2];
    }

    calibrationComplete =
        true;

    resetOrientation();

    Serial.println();
    Serial.println(
        "CALIBRATION COMPLETE"
    );

    Serial.println(
        "Neutral wrist position saved."
    );

    Serial.println();
}

// ============================================================
// START SYSTEM
// ============================================================

void startSystem()
{
    if (!calibrationComplete)
    {
        Serial.println(
            "ERROR: CALIBRATE IMUs FIRST"
        );

        return;
    }

    resetOrientation();

    if (!areServosEnabled()) {setupServos();}
    servoConnected[0] = areServosEnabled();
    servoConnected[1] = areServosEnabled();

    startTime_us =
        micros();

    systemRunning =
        true;

    Serial.println(
        "SYSTEM STARTED"
    );

    sendStatus();
}

// ============================================================
// STOP SYSTEM
// ============================================================

void stopSystem()
{
    systemRunning = false;
    
    if (areServosEnabled()) {
        originAngles();
        // Give the servos time to reach origin before detaching
        delay(300);
        endProgram(); // defined in servo_control.h
    }

    servoConnected[0] = false;
    servoConnected[1] = false;
    Serial.println(
        "SYSTEM STOPPED"
    );

    sendStatus();
}

// ============================================================
// SEND STATUS THROUGH BLE
//
// Format:
//
// STATUS,
//SYSTEM,
//BLE,
//HAND_IMU,
//FOREARM_IMU,
//SERVO1,
//SERVO2,
//CALIBRATED
// ============================================================

void sendStatus()
{
    if (
        statusCharacteristic == nullptr ||
        !bleConnected
    )
    {
        return;
    }

    String message =
        "STATUS,";

    message +=
        systemRunning
        ? "RUNNING,"
        : "STOPPED,";

    message +=
        bleConnected
        ? "CONNECTED,"
        : "DISCONNECTED,";

    message +=
        imuConnected[0]
        ? "CONNECTED,"
        : "DISCONNECTED,";

    message +=
        imuConnected[1]
        ? "CONNECTED,"
        : "DISCONNECTED,";

    message +=
        servoConnected[0]
        ? "CONNECTED,"
        : "DISCONNECTED,";

    message +=
        servoConnected[1]
        ? "CONNECTED,"
        : "DISCONNECTED,";

    message +=
        calibrationComplete
        ? "CALIBRATED"
        : "NOT_CALIBRATED";

    statusCharacteristic->setValue(
        message.c_str()
    );

    statusCharacteristic->notify();
}

// ============================================================
// SEND IMU DATA
//
// Format:
//
// DATA,timestamp,
//handRoll,handPitch,handYaw,
//forearmRoll,forearmPitch,forearmYaw,
//wristRoll,wristPitch,wristYaw
// ============================================================

void sendIMUData(
    int i,
    uint64_t timestamp_us
)
{

    if (
        dataCharacteristic == nullptr ||
        !bleConnected
    )
    {
        return;
    }

    if (i != 0)
    {
        return;
    }

    String message =
        "DATA,";

    message +=
        String(
            timestamp_us
        );

    message += ",";

    message +=
        String(
            orientation[0][0],
            2
        );

    message += ",";

    message +=
        String(
            orientation[0][1],
            2
        );

    message += ",";

    message +=
        String(
            orientation[0][2],
            2
        );

    message += ",";

    message +=
        String(
            orientation[1][0],
            2
        );

    message += ",";

    message +=
        String(
            orientation[1][1],
            2
        );

    message += ",";

    message +=
        String(
            orientation[1][2],
            2
        );

    message += ",";

    message +=
        String(
            relativeOrientation[0],
            2
        );

    message += ",";

    message +=
        String(
            relativeOrientation[1],
            2
        );

    message += ",";

    message +=
        String(
            relativeOrientation[2],
            2
        );

    dataCharacteristic->setValue(
        message.c_str()
    );

    dataCharacteristic->notify();
}

// ============================================================
// READ ALL IMUs
// ============================================================

void readIMUs()
{
    for (
        int i = 0;
        i < NUM_IMUS;
        i++
    )
    {
        if (!imuConnected[i])
        {
            continue;
        }

        if (imu[i].dataReady())
        {
            imu[i].getAGMT();

            uint64_t timestamp_us =
                micros() -
                startTime_us;

            float ax =
                imu[i].accX();

            float ay =
                imu[i].accY();

            float az =
                imu[i].accZ();

            float gx =
                imu[i].gyrX();

            float gy =
                imu[i].gyrY();

            float gz =
                imu[i].gyrZ();

            updateOrientation(
                i,
                ax,
                ay,
                az,
                gx,
                gy,
                gz,
                timestamp_us
            );
        }
    }

    // Calculate hand relative to forearm

    calculateRelativeOrientation();

    // Send BLE packet

    static uint64_t lastSendTime =
        0;

    uint64_t now =
        micros();

    if (
        now -
        lastSendTime
        >= 20000 // Bluetooth transmission frequency is 50 Hz (IMU sampling rate is still 100 Hz)
        // Just for visualization purposes but BLE gives errors over time if at 100 Hz
    )
    {
        lastSendTime =
            now;

        sendIMUData(
            0,
            now - startTime_us
        );
    }
}

// ============================================================
// INITIALIZE BLE
// ============================================================

void setupBLE()
{
    BLEDevice::init(
        "Tremor Stabilization Glove"
    );

    pServer =
        BLEDevice::createServer();

    pServer->setCallbacks(
        new ServerCallbacks()
    );

    BLEService* service =
        pServer->createService(
            SERVICE_UUID
        );

    // ========================================================
    // DATA CHARACTERISTIC
    // ========================================================

    dataCharacteristic =
        service->createCharacteristic(
            DATA_CHAR_UUID,
            BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_NOTIFY
        );

    dataCharacteristic->addDescriptor(
        new BLE2902()
    );

    // ========================================================
    // COMMAND CHARACTERISTIC
    // ========================================================

    commandCharacteristic =
        service->createCharacteristic(
            COMMAND_CHAR_UUID,
            BLECharacteristic::PROPERTY_WRITE
        );

    commandCharacteristic->setCallbacks(
        new CommandCallbacks()
    );

    // ========================================================
    // STATUS CHARACTERISTIC
    // ========================================================

    statusCharacteristic =
        service->createCharacteristic(
            STATUS_CHAR_UUID,
            BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_NOTIFY
        );

    statusCharacteristic->addDescriptor(
        new BLE2902()
    );

    // ========================================================
    // START SERVICE
    // ========================================================

    service->start();

    // ========================================================
    // START ADVERTISING
    // ========================================================

    BLEAdvertising* advertising =
        BLEDevice::getAdvertising();

    advertising->addServiceUUID(
        SERVICE_UUID
    );

    advertising->setScanResponse(
        true
    );

    advertising->setMinPreferred(
        0x06
    );

    advertising->setMinPreferred(
        0x12
    );

    BLEDevice::startAdvertising();

    Serial.println(
        "BLE ADVERTISING STARTED"
    );

    Serial.println(
        "Device: Tremor Stabilization Glove"
    );
}

// ============================================================
// SETUP
// ============================================================

void setup()
{
    Serial.begin(
        115200
    );

    delay(
        2000
    );

    Serial.println();
    Serial.println(
        "=============================="
    );

    Serial.println(
        "TREMOR STABILIZATION GLOVE"
    );

    Serial.println(
        "XIAO ESP32-S3"
    );

    Serial.println(
        "=============================="
    );

    // ========================================================
    // INITIALIZE SPI
    // ========================================================

    SPI.begin(
        SCK_PIN,
        MISO_PIN,
        MOSI_PIN
    );

    Serial.println(
        "SPI INITIALIZED"
    );

    // ========================================================
    // INITIALIZE IMUs
    // ========================================================

    for (
        int i = 0;
        i < NUM_IMUS;
        i++
    )
    {
        pinMode(
            CS_PINS[i],
            OUTPUT
        );

        digitalWrite(
            CS_PINS[i],
            HIGH
        );

        Serial.print(
            "Initializing "
        );

        Serial.print(
            IMU_NAMES[i]
        );

        Serial.println(
            "..."
        );

        imu[i].begin(
            CS_PINS[i],
            SPI
        );

        if (
            imu[i].status ==
            ICM_20948_Stat_Ok
        )
        {
            imuConnected[i] =
                true;

            Serial.print(
                IMU_NAMES[i]
            );

            Serial.println(
                " CONNECTED"
            );
        }
        else
        {
            imuConnected[i] =
                false;

            Serial.print(
                IMU_NAMES[i]
            );

            Serial.println(
                " FAILED"
            );

            Serial.println(
                imu[i].statusString()
            );
        }
    }

    // ========================================================
    // INITIALIZE BLE
    // ========================================================

    setupBLE();

    // ========================================================
    // SYSTEM READY
    // ========================================================

    Serial.println();
    Serial.println(
        "SYSTEM READY"
    );

    Serial.println(
        "Connect using Python BLE Dashboard."
    );

    Serial.println(
        "Run CALIBRATE before START."
    );
}

// ============================================================
// MAIN LOOP
// ============================================================

void loop()
{
    if (systemRunning)
    {
        readIMUs();
    }

    delay(
        1
    );
}

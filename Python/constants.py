# ============================================================
# Tremor Detection Dashboard
#
# File: constants.py
#
# Description:
# Global constants used throughout the dashboard.
# ============================================================

# ============================================================
# BLE SETTINGS
# ============================================================

DEVICE_NAME = "Tremor Stabilization Glove"

SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

DATA_CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

COMMAND_CHARACTERISTIC_UUID = "6e400002-b5a3-f393-e0a9-e50e24dcca9e"

STATUS_CHARACTERISTIC_UUID = "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

# ============================================================
# WINDOW
# ============================================================

WINDOW_TITLE = "Tremor Detection Dashboard"

WINDOW_WIDTH = 1500

WINDOW_HEIGHT = 900

# ============================================================
# GRAPH SETTINGS
# ============================================================

SAMPLE_RATE = 100                 # Hz

HISTORY_SECONDS = 10              # seconds displayed

BUFFER_SIZE = SAMPLE_RATE * HISTORY_SECONDS

GRAPH_UPDATE_MS = 20              # 50 FPS

# ============================================================
# PLOT RANGES
# ============================================================

WRIST_PITCH_RANGE = (-60, 60)

TREMOR_PITCH_RANGE = (-15, 15)

ENVELOPE_RANGE = (0, 15)

FREQUENCY_RANGE = (0, 15)

# ============================================================
# PACKET FORMAT
# ============================================================

PACKET_IDENTIFIER = "TREMOR"

PACKET_FIELDS = [
    "timestamp",
    "handPitch",
    "forearmPitch",
    "wristPitch",
    "tremorPitch",
    "envelope",
    "frequency",
    "confidence",
    "power",
    "mode",
    "detection"
]

# ============================================================
# STATUS
# ============================================================

STATE_MONITORING = 0

STATE_TREMOR = 1

STATUS_TEXT = {
    STATE_MONITORING: "Monitoring",
    STATE_TREMOR: "Tremor Detected"
}

# ============================================================
# CARD TITLES
# ============================================================

HAND_TITLE = "Hand Pitch"

FOREARM_TITLE = "Forearm Pitch"

WRIST_TITLE = "Wrist Pitch"

TREMOR_TITLE = "Tremor Pitch"

ENVELOPE_TITLE = "Envelope"

FREQUENCY_TITLE = "Frequency"

CONFIDENCE_TITLE = "Confidence"

POWER_TITLE = "Power"

MODE_TITLE = "Mode"

DETECTION_TITLE = "Detection"

STATUS_TITLE = "Status"

# ============================================================
# UNITS
# ============================================================

ANGLE_UNIT = "°"

FREQUENCY_UNIT = "Hz"

CONFIDENCE_UNIT = "%"

# ============================================================
# DEFAULT VALUES
# ============================================================

DEFAULT_FLOAT = 0.0

DEFAULT_STATUS = STATE_MONITORING
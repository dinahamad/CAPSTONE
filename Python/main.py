# ============================================================
# Tremor Detection Dashboard
#
# File: main.py
#
# Description:
# Application entry point.
# ============================================================

import sys

from PySide6.QtWidgets import QApplication
from PySide6.QtGui import QFont

from dashboard import Dashboard
#from serial_manager import SerialManager SERIAL COMS
from ble_manager import BLEManager


# ============================================================
# Update Dashboard
# ============================================================

def updateDashboard(packet):

    # Mode packet
    if packet["type"] == "mode":

        dashboard.setMode(packet["mode"])
        return

    # Power packet
    if packet["type"] == "power":

        dashboard.setPower(
            packet["power"]
        )
        return

    if packet["type"] == "hardware":

        dashboard.updateHardwareStatus(
            packet["handIMU"],
            packet["forearmIMU"],
            packet["topServo"],
            packet["bottomServo"]
        )

        return

    # Tremor packet
    if packet["type"] == "tremor":


        dashboard.updateData(
            packet["handPitch"],
            packet["forearmPitch"],
            packet["wristPitch"],
            packet["tremorPitch"],
            packet["envelope"],
            packet["frequency"],
            packet["confidence"],
            packet["detection"]
        )


# ============================================================
# Main
# ============================================================

app = QApplication(sys.argv)

app.setFont(QFont("Arial"))

dashboard = Dashboard()

dashboard.show()


# ------------------------------------------------------------
# Serial
# ------------------------------------------------------------

# serial = SerialManager() SERIAL COMS

# serial.dataReceived.connect(updateDashboard)

# serial.connectionChanged.connect(
#     dashboard.setConnected
# )

# serial.start()

ble = BLEManager()

ble.dataReceived.connect(updateDashboard)
ble.connectionChanged.connect(
    dashboard.setBluetoothConnected
)

ble.start()


# ------------------------------------------------------------
# Start Application
# ------------------------------------------------------------

exitCode = app.exec()

# serial.stop() SERIAL COMS

# serial.wait()

ble.stop()
ble.wait()

sys.exit(exitCode)
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

from dashboard import Dashboard
from serial_manager import SerialManager


# ============================================================
# Update Dashboard
# ============================================================

def updateDashboard(packet):

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

dashboard = Dashboard()

dashboard.show()


# ------------------------------------------------------------
# Serial
# ------------------------------------------------------------

serial = SerialManager()

serial.dataReceived.connect(updateDashboard)

serial.connectionChanged.connect(
    dashboard.setConnected
)

serial.start()


# ------------------------------------------------------------
# Start Application
# ------------------------------------------------------------

exitCode = app.exec()

serial.stop()

serial.wait()

sys.exit(exitCode)
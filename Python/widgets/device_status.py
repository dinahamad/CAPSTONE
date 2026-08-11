# ============================================================
# Tremor Detection Dashboard
#
# File: widgets/device_status.py
#
# Description:
# Displays hardware and system connection status.
# ============================================================

from PySide6.QtWidgets import (
    QFrame,
    QLabel,
    QVBoxLayout,
    QHBoxLayout,
    QGridLayout
)

from PySide6.QtCore import Qt
from PySide6.QtGui import QFont
import qtawesome as qta

from styles import *


class DeviceStatus(QFrame):

    def __init__(self):
        super().__init__()

        self.setObjectName("deviceStatus")

        self.setStyleSheet(f"""
            QFrame#deviceStatus {{
                background-color: {CARD};
                border: 1px solid #ECECEC;
                border-radius: 22px;
            }}

            QLabel {{
                background: transparent;
                border: none;
            }}
        """)

        self.buildUI()

    # ========================================================
    # BUILD UI
    # ========================================================

    def buildUI(self):

        mainLayout = QVBoxLayout(self)
        mainLayout.setAlignment(Qt.AlignTop)

        mainLayout.setContentsMargins(20, 15, 20, 15)
        mainLayout.setSpacing(10)

        # ----------------------------------------------------
        # Main title
        # ----------------------------------------------------

        title = QLabel("Device Status")
        title.setFixedHeight(35)

        titleFont = QFont("Arial")
        titleFont.setPointSize(16)
        titleFont.setBold(True)

        title.setFont(titleFont)
        title.setStyleSheet(f"color: {TEXT};")

        mainLayout.addWidget(title)

        # ----------------------------------------------------
        # Main status area
        # ----------------------------------------------------

        statusLayout = QHBoxLayout()

        # =========================
        # Sensors / Actuators
        # =========================

        hardwareLayout = QVBoxLayout()

        hardwareTitle = QLabel("Sensors & Actuators")
        hardwareTitle.setStyleSheet(f"""
            color: {PRIMARY};
            font-size: 12pt;
            font-weight: bold;
        """)

        hardwareLayout.addWidget(hardwareTitle)

        self.handIMU = self.createStatusRow(
            hardwareLayout,
            "fa5s.microchip",
            "Hand IMU"
        )

        self.forearmIMU = self.createStatusRow(
            hardwareLayout,
            "fa5s.microchip",
            "Forearm IMU"
        )

        self.topServo = self.createStatusRow(
            hardwareLayout,
            "fa5s.cog",
            "Top Wrist Servo"
        )

        self.bottomServo = self.createStatusRow(
            hardwareLayout,
            "fa5s.cog",
            "Bottom Wrist Servo"
        )

        # =========================
        # System connections
        # =========================

        connectionLayout = QVBoxLayout()

        connectionTitle = QLabel("System Connections")
        connectionTitle.setStyleSheet(f"""
            color: {PRIMARY};
            font-size: 12pt;
            font-weight: bold;
        """)

        connectionLayout.addWidget(connectionTitle)

        self.mcuLink = self.createStatusRow(
            connectionLayout,
            "fa5s.link",
            "MCU Link"
        )

        self.bluetooth = self.createStatusRow(
            connectionLayout,
            "fa5b.bluetooth-b",
            "Bluetooth"
        )
        connectionLayout.setAlignment(Qt.AlignTop)
        hardwareLayout.setAlignment(Qt.AlignTop)

        statusLayout.addLayout(hardwareLayout)
        statusLayout.addSpacing(20)
        statusLayout.addLayout(connectionLayout)

        mainLayout.addLayout(statusLayout)

        # # ----------------------------------------------------
        # # Power / Mode
        # # ----------------------------------------------------

        # bottomLayout = QGridLayout()

        # powerTitle = QLabel("Device Power")
        # powerTitle.setAlignment(Qt.AlignCenter)

        # modeTitle = QLabel("Current Mode")
        # modeTitle.setAlignment(Qt.AlignCenter)

        # self.powerLabel = QLabel("OFF")
        # self.powerLabel.setAlignment(Qt.AlignCenter)

        # self.modeLabel = QLabel("SENSING")
        # self.modeLabel.setAlignment(Qt.AlignCenter)

        # self.powerLabel.setStyleSheet(f"""
        #     color: {ERROR};
        #     font-size: 16pt;
        #     font-weight: bold;
        # """)

        # self.modeLabel.setStyleSheet(f"""
        #     color: {SECONDARY};
        #     font-size: 16pt;
        #     font-weight: bold;
        # """)

        # bottomLayout.addWidget(powerTitle, 0, 0)
        # bottomLayout.addWidget(modeTitle, 0, 1)

        # bottomLayout.addWidget(self.powerLabel, 1, 0)
        # bottomLayout.addWidget(self.modeLabel, 1, 1)

        # mainLayout.addLayout(bottomLayout)

    # ========================================================
    # CREATE STATUS ROW
    # ========================================================

    def createStatusRow(self, layout, iconName, name):

        row = QHBoxLayout()

        icon = QLabel()
        icon.setPixmap(
            qta.icon(
                iconName,
                color=PRIMARY
            ).pixmap(22, 22)
        )

        icon.setFixedWidth(30)
        icon.setAlignment(Qt.AlignCenter)

        label = QLabel(name)

        status = QLabel("● Disconnected")

        status.setStyleSheet(f"""
            color: {TEXT_SECONDARY};
            font-weight: bold;
        """)

        row.addWidget(icon)
        row.addWidget(label)
        row.addStretch()
        row.addWidget(status)

        layout.addLayout(row)

        return status

    # ========================================================
    # STATUS UPDATE
    # ========================================================

    def setConnection(self, label, connected):

        if connected:
            label.setText("● Connected")
            label.setStyleSheet(
                f"color: {SUCCESS}; font-weight: bold;"
            )

        else:
            label.setText("● Disconnected")
            label.setStyleSheet(
                f"color: {TEXT_SECONDARY}; font-weight: bold;"
            )

    def setHandIMU(self, connected):
        self.setConnection(self.handIMU, connected)

    def setForearmIMU(self, connected):
        self.setConnection(self.forearmIMU, connected)

    def setTopServo(self, connected):
        self.setConnection(self.topServo, connected)

    def setBottomServo(self, connected):
        self.setConnection(self.bottomServo, connected)

    def setMCUConnected(self, connected):
        self.setConnection(self.mcuLink, connected)

    def setBluetooth(self, connected):
        self.setConnection(self.bluetooth, connected)

    # ========================================================
    # RESET HARDWARE STATUS
    # ========================================================

    def resetHardware(self):

        self.setHandIMU(False)
        self.setForearmIMU(False)
        self.setTopServo(False)
        self.setBottomServo(False)
        self.setMCUConnected(False)
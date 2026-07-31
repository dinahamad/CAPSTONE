import sys
import math
import time
import os

from PySide6.QtGui import (
    QPixmap,
    QFont,
    QPainter,
    QPainterPath
)

from PySide6.QtCore import Qt, QTimer
from PySide6.QtGui import QPixmap, QFont
from PySide6.QtWidgets import (
    QApplication,
    QMainWindow,
    QWidget,
    QLabel,
    QPushButton,
    QFrame,
    QGridLayout,
    QHBoxLayout,
    QVBoxLayout,
    QSizePolicy
)

from styles import *
from widgets import *
from graphs import *
from arm_widget import *
from ble_manager import BLEManager, DashboardSignals


class TremorDashboard(QMainWindow):
    def circularPixmap(self, pixmap, size):

        scaled = pixmap.scaled(
            size,
            size,
            Qt.KeepAspectRatioByExpanding,
            Qt.SmoothTransformation
        )

        result = QPixmap(size, size)
        result.fill(Qt.transparent)

        painter = QPainter(result)
        painter.setRenderHint(QPainter.Antialiasing)

        path = QPainterPath()
        path.addEllipse(0, 0, size, size)

        painter.setClipPath(path)

        x = (scaled.width() - size) // 2
        y = (scaled.height() - size) // 2

        painter.drawPixmap(-x, -y, scaled)
        painter.end()

        return result

    def __init__(self):

        super().__init__()

        self.setWindowTitle("Tremor Tech Dashboard")

        self.resize(1600,950)

        self.setStyleSheet(STYLE_SHEET)

        self.timeData=[]
        self.rollData=[]
        self.pitchData=[]
        self.yawData=[]
        self.frequencyData=[]

        self.signals=DashboardSignals()

        self.ble=BLEManager(self.signals)

        self.buildUI()

        self.connectSignals()

        self.ble.start()

        self.timer=QTimer()

        self.timer.timeout.connect(self.updateGraphs)

        self.timer.start(40)


##############################################################
# UI
##############################################################

    def buildUI(self):

        central=QWidget()

        self.setCentralWidget(central)

        root=QVBoxLayout(central)

        root.setContentsMargins(20,20,20,20)

        root.setSpacing(18)

##############################################################
# HEADER
##############################################################

        header = QFrame()
        header.setStyleSheet(CARD_STYLE)

        headerLayout = QHBoxLayout(header)
        headerLayout.setContentsMargins(25, 15, 25, 15)
        headerLayout.setSpacing(20)

        ##############################################################
# LOGO
##############################################################

        self.logo = QLabel()

        logo_path = os.path.join(
            os.path.dirname(__file__),
            "assets",
            "Tremor_Tech_Logo_blue.png"
        )

        pix = QPixmap(logo_path)

        if not pix.isNull():

            self.logo.setPixmap(
                self.circularPixmap(pix, 90)
            )

        self.logo.setFixedSize(90, 90)
        self.logo.setAlignment(Qt.AlignCenter)
        

##############################################################
# TITLE
##############################################################

        titleLayout = QVBoxLayout()
        titleLayout.setSpacing(2)

        self.title = QLabel("Tremor Tech")
        self.title.setStyleSheet("""
        QLabel{
            color:#47046b;
            font-size:28px;
            font-weight:700;
        }
        """)

        self.subtitle = QLabel(
            "Wearable Stabillization Device for Tremors"
        )

        self.subtitle.setStyleSheet("""
        QLabel{
            color:#666666;
            font-size:14px;
        }
        """)

        titleLayout.addStretch()
        titleLayout.addWidget(self.title)
        titleLayout.addWidget(self.subtitle)
        titleLayout.addStretch()

##############################################################
# CONNECTION BADGE
##############################################################

        self.connectionBadge = QLabel("● DISCONNECTED")
        self.connectionBadge.setAlignment(Qt.AlignCenter)
        self.connectionBadge.setFixedSize(190, 45)

        self.connectionBadge.setStyleSheet("""
        QLabel{
            background:#E53935;
            color:white;
            border-radius:12px;
            font-size:13pt;
            font-weight:bold;
        }
        """)

##############################################################
# ASSEMBLE HEADER
##############################################################

        headerLayout.addWidget(self.logo)
        headerLayout.addLayout(titleLayout)
        headerLayout.addStretch()
        headerLayout.addWidget(self.connectionBadge)

        root.addWidget(header)

##############################################################
# STATUS CARDS
##############################################################

        statusLayout=QGridLayout()

        statusLayout.setHorizontalSpacing(15)

        statusLayout.setVerticalSpacing(15)

        self.handCard=StatusCard("IMU 1: Hand")

        self.forearmCard=StatusCard("IMU 2: Forearm")

        self.servo1Card=StatusCard("Servo 1: Dorsal Wrist")

        self.servo2Card=StatusCard("Servo 2: Palmar Wrist")

        self.calibrationCard=StatusCard("Calibration")

        self.xiaoCard=StatusCard("Microcontroller 1 (Sensing + Actuation)")

        statusLayout.addWidget(
            self.handCard,
            0,
            0
        )

        statusLayout.addWidget(
            self.forearmCard,
            0,
            1
        )

        statusLayout.addWidget(
            self.servo1Card,
            0,
            2
        )

        statusLayout.addWidget(
            self.servo2Card,
            0,
            3
        )

        statusLayout.addWidget(
            self.calibrationCard,
            0,
            4
        )

        statusLayout.addWidget(
            self.xiaoCard,
            0,
            5
        )

        root.addLayout(statusLayout)

##############################################################
# MAIN BODY
##############################################################

        body=QHBoxLayout()

##############################################################
# LEFT COLUMN
##############################################################

        left=QVBoxLayout()

        left.setSpacing(15)

##############################################################
# ARM
##############################################################

        self.arm=ArmWidget()

        left.addWidget(self.arm)

##############################################################
# BUTTONS
##############################################################

        controls=QFrame()

        controls.setStyleSheet(CARD_STYLE)

        controlLayout=QGridLayout(controls)

        self.calibrateButton=DashboardButton(
            "CALIBRATE",
            PRIMARY
        )

        self.startButton=DashboardButton(
            "START",
            SECONDARY
        )

        self.stopButton=DashboardButton(
            "STOP",
            ERROR
        )

        self.resetButton=DashboardButton(
            "RESET",
            "#777777"
        )

        controlLayout.addWidget(
            self.calibrateButton,
            0,
            0
        )

        controlLayout.addWidget(
            self.startButton,
            0,
            1
        )

        controlLayout.addWidget(
            self.stopButton,
            1,
            0
        )

        controlLayout.addWidget(
            self.resetButton,
            1,
            1
        )

        left.addWidget(controls)

##############################################################
# RIGHT COLUMN
##############################################################

        right=QVBoxLayout()

        values=QGridLayout()

        self.rollCard=ValueCard(
            "Roll",
            "°"
        )

        self.pitchCard=ValueCard(
            "Pitch",
            "°"
        )

        self.yawCard=ValueCard(
            "Yaw",
            "°"
        )

        self.batteryCard=BatteryCard()

        self.tremorCard=TremorCard()

        values.addWidget(
            self.rollCard,
            0,
            0
        )

        values.addWidget(
            self.pitchCard,
            0,
            1
        )

        values.addWidget(
            self.yawCard,
            0,
            2
        )

        values.addWidget(
            self.batteryCard,
            1,
            0
        )

        values.addWidget(
            self.tremorCard,
            1,
            1,
            1,
            2
        )

        right.addLayout(values)

##############################################################
# GRAPHS
##############################################################

        self.orientationGraph=OrientationGraph()

        self.tremorGraph=TremorGraph()

        right.addWidget(
            self.orientationGraph,
            3
        )

        right.addWidget(
            self.tremorGraph,
            2
        )

##############################################################
# FINISH LAYOUT
##############################################################

        body.addLayout(
            left,
            1
        )

        body.addLayout(
            right,
            2
        )

        root.addLayout(body)

##############################################################
# CONNECT SIGNALS
##############################################################

    def connectSignals(self):

        self.signals.connectionChanged.connect(
            self.updateConnection
        )

        self.signals.statusReceived.connect(
            self.updateStatus
        )

        self.signals.dataReceived.connect(
            self.processData
        )

        self.signals.errorReceived.connect(
            self.showError
        )

        self.signals.batteryReceived.connect(
            self.updateBattery
        )

        self.calibrateButton.clicked.connect(
            lambda: self.sendCommand("CALIBRATE")
        )

        self.startButton.clicked.connect(
            lambda: self.sendCommand("START")
        )

        self.stopButton.clicked.connect(
            lambda: self.sendCommand("STOP")
        )

        self.resetButton.clicked.connect(
            lambda: self.sendCommand("RESET")
        )


##############################################################
# SEND COMMAND
##############################################################

    def sendCommand(self, command):

        self.ble.sendCommand(command)


##############################################################
# CONNECTION STATUS
##############################################################

    def updateConnection(self, connected):

        if connected:

            self.connectionBadge.setText(
                "● CONNECTED"
            )

            self.connectionBadge.setStyleSheet("""
            QLabel{
                background:#43A047;
                color:white;
                border-radius:10px;
                padding:10px;
                font-size:12pt;
                font-weight:bold;
            }
            """)

            self.xiaoCard.setStatus(
                "CONNECTED"
            )

        else:

            self.connectionBadge.setText(
                "● DISCONNECTED"
            )

            self.connectionBadge.setStyleSheet("""
            QLabel{
                background:#E53935;
                color:white;
                border-radius:10px;
                padding:10px;
                font-size:12pt;
                font-weight:bold;
            }
            """)

            self.xiaoCard.setStatus(
                "DISCONNECTED"
            )


##############################################################
# BATTERY
##############################################################

    def updateBattery(self, percent):

        self.batteryCard.setBattery(
            percent
        )


##############################################################
# STATUS PARSER
##############################################################

    def updateStatus(self, message):

        if not message.startswith("STATUS,"):

            return

        parts = message.split(",")

        if len(parts) < 8:

            return

        self.handCard.setStatus(parts[3])

        self.forearmCard.setStatus(parts[4])

        self.servo1Card.setStatus(parts[5])

        self.servo2Card.setStatus(parts[6])

        self.calibrationCard.setStatus(parts[7])

        self.arm.setHandConnected(
            parts[3].upper() == "CONNECTED"
            or
            parts[3].upper() == "READY"
        )

        self.arm.setForearmConnected(
            parts[4].upper() == "CONNECTED"
            or
            parts[4].upper() == "READY"
        )


##############################################################
# PROCESS IMU DATA
##############################################################

    def processData(self, message):

        if not message.startswith("DATA,"):

            return

        parts = message.split(",")

        if len(parts) < 11:

            return

        try:

            timestamp = float(parts[1]) / 1000000

            roll = float(parts[8])

            pitch = float(parts[9])

            yaw = float(parts[10])

        except:

            return

        self.timeData.append(timestamp)

        self.rollData.append(roll)

        self.pitchData.append(pitch)

        self.yawData.append(yaw)

        if len(self.timeData) > 500:

            self.timeData = self.timeData[-500:]

            self.rollData = self.rollData[-500:]

            self.pitchData = self.pitchData[-500:]

            self.yawData = self.yawData[-500:]

        self.rollCard.setValue(
            f"{roll:.1f}"
        )

        self.pitchCard.setValue(
            f"{pitch:.1f}"
        )

        self.yawCard.setValue(
            f"{yaw:.1f}"
        )

        self.arm.updateOrientation(
            roll,
            pitch,
            yaw
        )

        tremor = abs(roll)

        if len(self.rollData) > 25:

            recent = self.rollData[-25:]

            tremor = (
                max(recent) -
                min(recent)
            )

        frequency = min(
            12,
            max(
                0,
                tremor / 5
            )
        )

        self.frequencyData.append(
            frequency
        )

        if len(self.frequencyData) > 500:

            self.frequencyData = (
                self.frequencyData[-500:]
            )

        self.tremorCard.setFrequency(
            frequency
        )


##############################################################
# UPDATE GRAPHS
##############################################################

    def updateGraphs(self):

        if len(self.timeData) < 2:

            return

        self.orientationGraph.updateGraph(

            self.timeData,

            self.rollData,

            self.pitchData,

            self.yawData

        )

        self.tremorGraph.updateGraph(

            self.timeData,

            self.frequencyData

        )


##############################################################
# ERROR
##############################################################

    def showError(self, error):

        print(error)


##############################################################
# MAIN
##############################################################

def main():

    app = QApplication(sys.argv)

    window = TremorDashboard()

    window.show()

    sys.exit(app.exec())


if __name__ == "__main__":

    main()

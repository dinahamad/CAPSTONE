"""
============================================================
TremorTech Animated Arm Widget
============================================================
"""

from PySide6.QtWidgets import QWidget
from PySide6.QtGui import (
    QPainter,
    QColor,
    QPen,
    QBrush,
    QFont
)
from PySide6.QtCore import Qt

from styles import *


class ArmWidget(QWidget):

    def __init__(self):

        super().__init__()

        self.setMinimumSize(350, 500)

        self.roll = 0
        self.pitch = 0
        self.yaw = 0

        self.handConnected = False
        self.forearmConnected = False

    # ========================================================

    def updateOrientation(self, roll, pitch, yaw):

        self.roll = roll
        self.pitch = pitch
        self.yaw = yaw

        self.update()

    # ========================================================

    def setHandConnected(self, connected):

        self.handConnected = connected

        self.update()

    # ========================================================

    def setForearmConnected(self, connected):

        self.forearmConnected = connected

        self.update()

    # ========================================================

    def paintEvent(self, event):

        painter = QPainter(self)

        painter.setRenderHint(QPainter.Antialiasing)

        painter.fillRect(
            self.rect(),
            QColor(BACKGROUND)
        )

        painter.translate(
            self.width()/2,
            self.height()/2 + 70
        )

        painter.rotate(-self.roll)

        # ----------------------------------------------------
        # FOREARM
        # ----------------------------------------------------

        painter.setPen(Qt.NoPen)

        painter.setBrush(
            QColor(SECONDARY)
        )

        painter.drawRoundedRect(
            -18,
            -170,
            36,
            170,
            18,
            18
        )

        # ----------------------------------------------------
        # HAND
        # ----------------------------------------------------

        painter.setBrush(
            QColor(PRIMARY)
        )

        painter.drawRoundedRect(
            -22,
            -245,
            44,
            75,
            18,
            18
        )

        # ----------------------------------------------------
        # WRIST JOINT
        # ----------------------------------------------------

        painter.setBrush(
            QColor("#DDDDDD")
        )

        painter.drawEllipse(
            -12,
            -182,
            24,
            24
        )

        # ----------------------------------------------------
        # HAND IMU
        # ----------------------------------------------------

        if self.handConnected:

            colour = QColor(SUCCESS)

        else:

            colour = QColor(ERROR)

        painter.setBrush(colour)

        painter.drawEllipse(
            -8,
            -220,
            16,
            16
        )

        # ----------------------------------------------------
        # FOREARM IMU
        # ----------------------------------------------------

        if self.forearmConnected:

            colour = QColor(SUCCESS)

        else:

            colour = QColor(ERROR)

        painter.setBrush(colour)

        painter.drawEllipse(
            -8,
            -90,
            16,
            16
        )

        # ----------------------------------------------------
        # LABELS
        # ----------------------------------------------------

        painter.setPen(
            QColor(TEXT)
        )

        font = QFont()

        font.setPointSize(10)

        painter.setFont(font)

        painter.drawText(
            30,
            -208,
            "Hand IMU"
        )

        painter.drawText(
            30,
            -78,
            "Forearm IMU"
        )

        painter.drawText(
            -60,
            45,
            f"Roll : {self.roll:.1f}°"
        )

        painter.drawText(
            -60,
            65,
            f"Pitch : {self.pitch:.1f}°"
        )

        painter.drawText(
            -60,
            85,
            f"Yaw : {self.yaw:.1f}°"
        )

        painter.end()
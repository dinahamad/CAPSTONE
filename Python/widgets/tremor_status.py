# ============================================================
# Tremor Detection Dashboard
#
# File: widgets/tremor_status.py
#
# Description:
# Displays tremor algorithm status.
# ============================================================

from PySide6.QtWidgets import (
    QFrame,
    QLabel,
    QVBoxLayout,
    QHBoxLayout
)

from PySide6.QtCore import Qt
from PySide6.QtGui import QFont

from styles import *


class TremorStatus(QFrame):

    def __init__(self):
        super().__init__()

        self.setObjectName("tremorStatus")

        self.setStyleSheet(f"""
            QFrame#tremorStatus {{
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
        mainLayout.setSpacing(12)

        # ----------------------------------------------------
        # Title
        # ----------------------------------------------------

        title = QLabel("Tremor Algorithm / Detection")
        title.setFixedHeight(35)

        titleFont = QFont("Arial")
        titleFont.setPointSize(16)
        titleFont.setBold(True)

        title.setFont(titleFont)
        title.setStyleSheet(f"color: {TEXT};")

        mainLayout.addWidget(title)

        # ----------------------------------------------------
        # Algorithm checks
        # ----------------------------------------------------

        checksLayout = QHBoxLayout()

        self.amplitudeCard = self.createCheckCard(
            "Amplitude Check",
            "≥ 0.30°"
        )

        self.frequencyCard = self.createCheckCard(
            "Frequency Check",
            "4.00 Hz – 8.00 Hz"
        )

        checksLayout.addWidget(self.amplitudeCard["frame"])
        checksLayout.addWidget(self.frequencyCard["frame"])

        mainLayout.addLayout(checksLayout)

        # ----------------------------------------------------
        # Detection result
        # ----------------------------------------------------

        self.resultFrame = QFrame()

        self.resultFrame.setStyleSheet(f"""
            background-color: {BACKGROUND};
            border-radius: 14px;
        """)

        resultLayout = QVBoxLayout(self.resultFrame)

        resultTitle = QLabel("Tremor Detection Result")
        resultTitle.setAlignment(Qt.AlignCenter)

        self.resultLabel = QLabel("MONITORING")
        self.resultLabel.setAlignment(Qt.AlignCenter)

        self.resultLabel.setStyleSheet(f"""
            color: {TEXT_SECONDARY};
            font-size: 18pt;
            font-weight: bold;
        """)

        resultLayout.addWidget(resultTitle)
        resultLayout.addWidget(self.resultLabel)

        mainLayout.addWidget(self.resultFrame)

    # ========================================================
    # CHECK CARD
    # ========================================================

    def createCheckCard(self, title, rangeText):

        frame = QFrame()

        frame.setStyleSheet(f"""
            background-color: {BACKGROUND};
            border-radius: 14px;
        """)

        layout = QVBoxLayout(frame)

        titleLabel = QLabel(title)
        titleLabel.setAlignment(Qt.AlignCenter)

        statusLabel = QLabel("OUT OF RANGE")
        statusLabel.setAlignment(Qt.AlignCenter)

        statusLabel.setStyleSheet(f"""
            color: {ERROR};
            font-size: 15pt;
            font-weight: bold;
        """)

        rangeLabel = QLabel(rangeText)
        rangeLabel.setAlignment(Qt.AlignCenter)

        rangeLabel.setStyleSheet(
            f"color: {TEXT_SECONDARY};"
        )

        layout.addWidget(titleLabel)
        layout.addWidget(statusLabel)
        layout.addWidget(rangeLabel)

        return {
            "frame": frame,
            "status": statusLabel
        }

    # ========================================================
    # UPDATE
    # ========================================================

    def updateStatus(
        self,
        amplitude,
        frequency,
        detection
    ):

        amplitudeValid = amplitude >= 0.30
        frequencyValid = 4.0 <= frequency <= 8.0

        # Amplitude
        if amplitudeValid:
            self.amplitudeCard["status"].setText(
                "IN RANGE"
            )

            self.amplitudeCard["status"].setStyleSheet(
                f"color: {SUCCESS}; font-size: 15pt; font-weight: bold;"
            )

        else:
            self.amplitudeCard["status"].setText(
                "OUT OF RANGE"
            )

            self.amplitudeCard["status"].setStyleSheet(
                f"color: {ERROR}; font-size: 15pt; font-weight: bold;"
            )

        # Frequency
        if frequencyValid:
            self.frequencyCard["status"].setText(
                "IN RANGE"
            )

            self.frequencyCard["status"].setStyleSheet(
                f"color: {SUCCESS}; font-size: 15pt; font-weight: bold;"
            )

        else:
            self.frequencyCard["status"].setText(
                "OUT OF RANGE"
            )

            self.frequencyCard["status"].setStyleSheet(
                f"color: {ERROR}; font-size: 15pt; font-weight: bold;"
            )

        # Detection
        if detection == 1:

            self.resultLabel.setText(
                "TREMOR DETECTED"
            )

            self.resultLabel.setStyleSheet(
                f"color: {SUCCESS}; font-size: 18pt; font-weight: bold;"
            )

        else:

            self.resultLabel.setText(
                "MONITORING"
            )

            self.resultLabel.setStyleSheet(
                f"color: {TEXT_SECONDARY}; font-size: 18pt; font-weight: bold;"
            )

    def setPower(self, power):

        if not power:

            self.amplitudeCard["status"].setText("----------")
            self.frequencyCard["status"].setText("----------")
            self.resultLabel.setText("----------")

            self.amplitudeCard["status"].setStyleSheet(
                f"color: {TEXT_SECONDARY}; font-size: 15pt; font-weight: bold;"
            )

            self.frequencyCard["status"].setStyleSheet(
                f"color: {TEXT_SECONDARY}; font-size: 15pt; font-weight: bold;"
            )

            self.resultLabel.setStyleSheet(
                f"color: {TEXT_SECONDARY}; font-size: 18pt; font-weight: bold;"
            )
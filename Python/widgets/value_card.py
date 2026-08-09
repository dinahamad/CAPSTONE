# ============================================================
# Tremor Detection Dashboard
#
# File: widgets/value_card.py
#
# Description:
# Reusable value card widget.
# Used for:
# - Hand Pitch
# - Forearm Pitch
# - Wrist Pitch
# - Tremor Pitch
# - Envelope
# - Frequency
# - Confidence
# - Status
# ============================================================

from PySide6.QtWidgets import (
    QFrame,
    QLabel,
    QVBoxLayout
)

from PySide6.QtGui import QFont
from PySide6.QtCore import Qt

from styles import *


class ValueCard(QFrame):

    def __init__(self, title, value="0.00", units=""):

        super().__init__()

        self.units = units

        self.setStyleSheet(CARD_STYLE)

        self.buildUI(title, value)

    # ========================================================
    # Build UI
    # ========================================================

    def buildUI(self, title, value):

        layout = QVBoxLayout(self)

        layout.setContentsMargins(25, 22, 25, 22)

        layout.setSpacing(16)

        # ----------------------------------------------------
        # Title
        # ----------------------------------------------------

        self.titleLabel = QLabel(title)

        titleFont = QFont()

        titleFont.setPointSize(15)

        titleFont.setBold(True)

        self.titleLabel.setFont(titleFont)

        self.titleLabel.setAlignment(Qt.AlignCenter)

        self.titleLabel.setStyleSheet(f"""
        color:{TEXT_SECONDARY};
        background: transparent;
        border:none;
        """)

        layout.addWidget(self.titleLabel)

        # ----------------------------------------------------
        # Value
        # ----------------------------------------------------

        self.valueLabel = QLabel(value)

        valueFont = QFont()

        valueFont.setPointSize(28)

        valueFont.setBold(True)

        self.valueLabel.setFont(valueFont)

        self.valueLabel.setAlignment(Qt.AlignCenter)

        self.valueLabel.setStyleSheet(f"""
        color:{PRIMARY};
        background: transparent;
        border:none;
        """)

        layout.addWidget(self.valueLabel)

    # ========================================================
    # Update Value
    # ========================================================

    def setValue(self, value):

        if isinstance(value, float):

            self.valueLabel.setText(
                f"{value:.2f}{self.units}"
            )

        else:

            self.valueLabel.setText(
                str(value)
            )

    # ========================================================
    # Frequency Colour
    # ========================================================

    # Updates the frequency value and turns it green when within 4-8 Hz.
    def setFrequency(self, frequency):

        self.setValue(frequency)

        if 4.0 <= frequency <= 8.0:
            colour = SUCCESS
        else:
            colour = PRIMARY

        self.valueLabel.setStyleSheet(f"""
            color:{colour};
            background: transparent;
            border:none;
        """)

    # ========================================================
    # Envelope Colour
    # ========================================================

    # Updates the envelope value and turns it green when above the tremor threshold.
    def setEnvelope(self, envelope):

        self.setValue(envelope)

        if envelope >= 0.3:
            colour = SUCCESS
        else:
            colour = PRIMARY

        self.valueLabel.setStyleSheet(f"""
            color:{colour};
            background: transparent;
            border:none;
        """)

    # ========================================================
    # Status Card Colour
    # ========================================================

    def setPower(self, state):

        if state == 0:
            colour = ERROR
            text = "Off"
        else:
            colour = SUCCESS
            text = "On"

        self.valueLabel.setText(text)

        self.valueLabel.setStyleSheet(f"""
            color:{colour};
            font-size:{CARD_VALUE_SIZE}pt;
            font-weight:bold;
        """)


    def setMode(self, state):

        if state == 0:
            colour = PRIMARY
            text = "Sensing"
        else:
            colour = SECONDARY
            text = "Stabilizing"

        self.valueLabel.setText(text)

        self.valueLabel.setStyleSheet(f"""
            color:{colour};
            font-size:{CARD_VALUE_SIZE}pt;
            font-weight:bold;
        """)


    def setDetection(self, state):

        if state == 0:
            colour = WARNING
            text = "Monitoring"
        else:
            colour = SUCCESS
            text = "Tremor Detected"

        self.valueLabel.setText(text)

        self.valueLabel.setStyleSheet(f"""
            color:{colour};
            font-size:{CARD_VALUE_SIZE}pt;
            font-weight:bold;
        """)
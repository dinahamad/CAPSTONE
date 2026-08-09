# ============================================================
# Tremor Detection Dashboard
#
# File: dashboard.py
#
# Description:
# Main dashboard window.
# ============================================================

from PySide6.QtWidgets import (
    QMainWindow,
    QWidget,
    QVBoxLayout,
    QGridLayout
)

from widgets.header import Header
from widgets.value_card import ValueCard
from widgets.live_plot import LivePlot

from constants import *
from styles import *


class Dashboard(QMainWindow):

    def __init__(self):

        super().__init__()

        self.setWindowTitle(WINDOW_TITLE)

        self.resize(
            WINDOW_WIDTH,
            WINDOW_HEIGHT
        )

        self.setStyleSheet(MAIN_STYLE)

        self.buildUI()

    # ========================================================
    # BUILD UI
    # ========================================================

    def buildUI(self):

        central = QWidget()

        self.setCentralWidget(central)

        self.mainLayout = QVBoxLayout(central)

        self.mainLayout.setContentsMargins(
            15,
            15,
            15,
            15
        )

        self.mainLayout.setSpacing(15)

        # ----------------------------------------------------
        # Header
        # ----------------------------------------------------

        self.header = Header()

        self.mainLayout.addWidget(
            self.header
        )

        # ----------------------------------------------------
        # Value Cards
        # ----------------------------------------------------

        self.cardLayout = QGridLayout()

        self.cardLayout.setHorizontalSpacing(12)

        self.cardLayout.setVerticalSpacing(12)

        self.handCard = ValueCard(
            HAND_TITLE,
            units=ANGLE_UNIT
        )

        self.forearmCard = ValueCard(
            FOREARM_TITLE,
            units=ANGLE_UNIT
        )

        self.wristCard = ValueCard(
            WRIST_TITLE,
            units=ANGLE_UNIT
        )

        self.tremorCard = ValueCard(
            TREMOR_TITLE,
            units=ANGLE_UNIT
        )

        self.envelopeCard = ValueCard(
            ENVELOPE_TITLE,
            units=ANGLE_UNIT
        )

        self.frequencyCard = ValueCard(
            FREQUENCY_TITLE,
            units=" Hz"
        )

        self.confidenceCard = ValueCard(
            CONFIDENCE_TITLE,
            units="%"
        )

        self.statusCard = ValueCard(
            STATUS_TITLE
        )

        # First row

        self.cardLayout.addWidget(
            self.handCard,
            0,
            0
        )

        self.cardLayout.addWidget(
            self.forearmCard,
            0,
            1
        )

        self.cardLayout.addWidget(
            self.wristCard,
            0,
            2
        )

        self.cardLayout.addWidget(
            self.tremorCard,
            0,
            3
        )

        # Second row

        self.cardLayout.addWidget(
            self.envelopeCard,
            1,
            0
        )

        self.cardLayout.addWidget(
            self.frequencyCard,
            1,
            1
        )

        self.cardLayout.addWidget(
            self.confidenceCard,
            1,
            2
        )

        self.cardLayout.addWidget(
            self.statusCard,
            1,
            3
        )

        self.mainLayout.addLayout(
            self.cardLayout
        )

                # ----------------------------------------------------
        # Live Graphs
        # ----------------------------------------------------

        self.wristPlot = LivePlot(
            title="Relative Wrist Pitch",
            colour=WRIST_COLOUR,
            yRange=WRIST_PITCH_RANGE,
            history=BUFFER_SIZE
        )

        self.tremorPlot = LivePlot(
            title="Tremor Pitch (Band-pass)",
            colour=TREMOR_COLOUR,
            yRange=TREMOR_PITCH_RANGE,
            history=BUFFER_SIZE
        )

        self.envelopePlot = LivePlot(
            title="Envelope",
            colour=ENVELOPE_COLOUR,
            yRange=ENVELOPE_RANGE,
            history=BUFFER_SIZE
        )

        self.frequencyPlot = LivePlot(
            title="Frequency",
            colour=FREQUENCY_COLOUR,
            yRange=FREQUENCY_RANGE,
            history=BUFFER_SIZE
        )

        # ----------------------------------------------------
        # Live Graph Layout (2 x 2)
        # ----------------------------------------------------

        self.graphLayout = QGridLayout()
        self.graphLayout.setHorizontalSpacing(12)
        self.graphLayout.setVerticalSpacing(12)

        self.graphLayout.addWidget(self.wristPlot,     0, 0)
        self.graphLayout.addWidget(self.tremorPlot,    0, 1)
        self.graphLayout.addWidget(self.envelopePlot,  1, 0)
        self.graphLayout.addWidget(self.frequencyPlot, 1, 1)

        # Make both columns grow equally
        self.graphLayout.setColumnStretch(0, 1)
        self.graphLayout.setColumnStretch(1, 1)

        # Make both rows grow equally
        self.graphLayout.setRowStretch(0, 1)
        self.graphLayout.setRowStretch(1, 1)

        self.mainLayout.addLayout(self.graphLayout)

    # ========================================================
    # Update Dashboard
    # ========================================================

    def updateData(
        self,
        handPitch,
        forearmPitch,
        wristPitch,
        tremorPitch,
        envelope,
        frequency,
        confidence,
        detection
    ):

        # -----------------------------
        # Value Cards
        # -----------------------------

        self.handCard.setValue(handPitch)

        self.forearmCard.setValue(forearmPitch)

        self.wristCard.setValue(wristPitch)

        self.tremorCard.setValue(tremorPitch)

        self.envelopeCard.setEnvelope(envelope)

        self.frequencyCard.setFrequency(frequency)

        self.confidenceCard.setValue(confidence)

        self.statusCard.setDetection(detection)

        # -----------------------------
        # Live Graphs
        # -----------------------------

        self.wristPlot.updateValue(wristPitch)

        self.tremorPlot.updateValue(tremorPitch)

        self.envelopePlot.updateValue(envelope)

        self.frequencyPlot.updateValue(frequency)

    # ========================================================
    # BLE Status
    # ========================================================

    def setConnected(self, connected):

        self.header.setConnected(connected)

    # ========================================================
    # Reset Graphs
    # ========================================================

    def clearGraphs(self):

        self.wristPlot.clear()

        self.tremorPlot.clear()

        self.envelopePlot.clear()

        self.frequencyPlot.clear()
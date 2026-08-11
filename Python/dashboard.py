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

from collections import deque
import time

from PySide6.QtCore import QTimer
from widgets.header import Header
from widgets.live_plot import LivePlot
from widgets.device_status import DeviceStatus
from widgets.tremor_status import TremorStatus

from constants import *
from styles import *


class Dashboard(QMainWindow):

    def __init__(self):

        super().__init__()

        # Tremor amplitude comparison
        self.power = False
        self.mode = 0                  # 0 = Sensing, 1 = Stabilizing
        self.previousMode = 0

        self.sensingAmplitudes = deque(maxlen=100)
        self.stabilizingAmplitudes = deque(maxlen=100)

        self.sensingBaseline = 0.0
        self.lastHardwareUpdate = None

        self.setWindowTitle(WINDOW_TITLE)

        self.resize(
            WINDOW_WIDTH,
            WINDOW_HEIGHT
        )

        self.setStyleSheet(MAIN_STYLE)

        self.buildUI()
        self.hardwareTimer = QTimer(self)
        self.hardwareTimer.timeout.connect(self.checkHardwareTimeout)
        self.hardwareTimer.start(500)


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
        # Status Panels
        # ----------------------------------------------------

        self.statusLayout = QGridLayout()

        self.statusLayout.setHorizontalSpacing(12)

        self.deviceStatus = DeviceStatus()
        self.tremorStatus = TremorStatus()

        self.statusLayout.addWidget(
            self.deviceStatus,
            0,
            0
        )

        self.statusLayout.addWidget(
            self.tremorStatus,
            0,
            1
        )

        self.statusLayout.setColumnStretch(0, 1)
        self.statusLayout.setColumnStretch(1, 1)

        self.mainLayout.addLayout(
            self.statusLayout
        )

        # ----------------------------------------------------
        # Live Graphs
        # ----------------------------------------------------

        self.wristPlot = LivePlot(
            title="Wrist Motion",
            colour=SECONDARY,
            yRange=WRIST_PITCH_RANGE,
            history=BUFFER_SIZE,
            units="Wrist Pitch (°)",
            valueTitle="Wrist Pitch",
            valueUnit="°"
        )

        self.tremorPlot = LivePlot(
            title="Tremor Motion", # This was Tremor Pitch (Band-pass)
            colour=SECONDARY,
            yRange=TREMOR_PITCH_RANGE,
            history=BUFFER_SIZE,
            units="Tremor Pitch (°)",
            valueTitle="Tremor Pitch",
            valueUnit="°"
        )

        self.envelopePlot = LivePlot(
            title="Tremor Amplitude", # This was Envelope
            colour=SECONDARY,
            yRange=ENVELOPE_RANGE,
            history=BUFFER_SIZE,
            units="Amplitude (°)",
            valueTitle="Tremor Amplitude", 
            valueUnit="°",
            comparison=True
        )

        self.frequencyPlot = LivePlot(
            title="Tremor Frequency", 
            colour=SECONDARY,
            yRange=FREQUENCY_RANGE,
            history=BUFFER_SIZE,
            units="Frequency (Hz)",
            valueTitle="Frequency", 
            valueUnit=" Hz"
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



    def setMode(self, mode):

        # Sensing -> Stabilizing
        if self.mode == 0 and mode == 1:

            if self.sensingAmplitudes:

                self.sensingBaseline = (
                    sum(self.sensingAmplitudes)
                    / len(self.sensingAmplitudes)
                )

            self.stabilizingAmplitudes.clear()

        # Stabilizing -> Sensing
        elif self.mode == 1 and mode == 0:

            self.sensingAmplitudes.clear()
            self.stabilizingAmplitudes.clear()

        self.previousMode = self.mode

        if mode != self.mode:

            self.wristPlot.setMode(mode)
            self.tremorPlot.setMode(mode)
            self.envelopePlot.setMode(mode)
            self.frequencyPlot.setMode(mode)


        self.mode = mode

        self.header.setMode(mode)

        # Set plot colour based on mode
        if mode == 0:
            colour = SECONDARY
        else:
            colour = PRIMARY


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
        # Tremor Algorithm Status
        # -----------------------------

        self.tremorStatus.updateStatus(
            envelope,
            frequency,
            detection
        )

        # -----------------------------
        # Live Graphs
        # -----------------------------

        self.wristPlot.updateValue(wristPitch)

        self.tremorPlot.updateValue(tremorPitch)
        self.envelopePlot.updateValue(envelope)

        # Tremor amplitude comparison
        if self.mode == 0:

            # Store recent sensing values
            self.sensingAmplitudes.append(envelope)

            sensingAverage = (
                sum(self.sensingAmplitudes)
                / len(self.sensingAmplitudes)
            )

            self.envelopePlot.updateComparison(
                sensingAverage
            )

        elif self.mode == 1:

            # Store stabilization values
            self.stabilizingAmplitudes.append(envelope)

            stabilizingAverage = (
                sum(self.stabilizingAmplitudes)
                / len(self.stabilizingAmplitudes)
            )

            if self.sensingBaseline > 0:

                reduction = (
                    (self.sensingBaseline - stabilizingAverage)
                    / self.sensingBaseline
                ) * 100

                self.envelopePlot.updateComparison(
                    self.sensingBaseline,
                    stabilizingAverage,
                    reduction
                )

        self.frequencyPlot.updateValue(frequency)

    # ========================================================
    # BLE Status
    # ========================================================

    def setConnected(self, connected):
        pass

    # ========================================================
    # Reset Graphs
    # ========================================================

    def clearGraphs(self):

        self.wristPlot.clear()

        self.tremorPlot.clear()

        self.envelopePlot.clear()

        self.frequencyPlot.clear()

    # ========================================================
    # POWER STATUS
    # ========================================================
    def setPower(self, power):

        # Only reset when actually transitioning OFF -> ON
        if power and not self.power:
            self.clearGraphs()

        self.power = power

        self.header.setPower(power)

        self.tremorStatus.setPower(power)

        if power:
            self.deviceStatus.setMCUConnected(True)
        else:
            self.deviceStatus.resetHardware()

    def updateHardwareStatus(
        self,
        handIMU,
        forearmIMU,
        topServo,
        bottomServo
    ):
        self.lastHardwareUpdate = time.time()

        self.deviceStatus.setHandIMU(handIMU)
        self.deviceStatus.setForearmIMU(forearmIMU)
        self.deviceStatus.setTopServo(topServo)
        self.deviceStatus.setBottomServo(bottomServo)


    def checkHardwareTimeout(self):

        if self.lastHardwareUpdate is None:
            return

        if time.time() - self.lastHardwareUpdate > 2.5:
            self.deviceStatus.resetHardware()

    def setBluetoothConnected(self, connected):

        self.deviceStatus.setBluetooth(connected)

        # MCU is considered connected when BLE is connected
        self.deviceStatus.setMCUConnected(connected)
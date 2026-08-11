# ============================================================
# Tremor Detection Dashboard
#
# File: widgets/live_plot.py
#
# Description:
# Reusable scrolling graph widget.
# ============================================================

from collections import deque

import pyqtgraph as pg

from PySide6.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout, QFrame, QLabel
from PySide6.QtCore import Qt
from styles import *

from PySide6.QtGui import QFont


class LivePlot(QWidget):

    def __init__(
        self,
        title,
        colour,
        yRange=(-10, 10),
        history=1000,
        units="",
        valueTitle="",
        valueUnit="",
        comparison=False
    ):

        super().__init__()
        self.valueUnit = valueUnit
        self.comparison = comparison


        self.history = history
        self.sampleRate = 50

        self.data = deque(maxlen=history)
        self.time = deque(maxlen=history)
        self.currentTime = 0.0

        outerLayout = QVBoxLayout(self)
        outerLayout.setContentsMargins(0, 0, 0, 0)

        self.card = QFrame()
        self.card.setObjectName("plotCard")

        self.card.setStyleSheet("""
            QFrame#plotCard {
                background-color: white;
                border-radius: 18px;
            }
        """)

        outerLayout.addWidget(self.card)

        layout = QHBoxLayout(self.card)
        layout.setContentsMargins(10, 10, 10, 10)

        # ----------------------------------------------------
        # Graph
        # ----------------------------------------------------

        self.plot = pg.PlotWidget()
        self.valueBox = QFrame()
        self.valueBox.setFixedWidth(140)

        self.valueBox.setStyleSheet(f"""
            background-color: {BACKGROUND};
            border-radius: 12px;
        """)

        valueLayout = QVBoxLayout(self.valueBox)
        valueLayout.setAlignment(Qt.AlignCenter)
        valueLayout.setSpacing(10)
        valueLayout.setContentsMargins(10, 10, 10, 10)

        self.valueTitle = QLabel(title)
        self.valueTitle.setAlignment(Qt.AlignCenter)

        self.valueTitle.setStyleSheet(f"""
            color: {TEXT};
            font-size: 14pt;
            font-weight: bold;
        """)

        valueLayout.addWidget(self.valueTitle)

        if self.comparison:

            self.sensingTitle = QLabel("Sensing")
            self.sensingTitle.setAlignment(Qt.AlignCenter)

            self.sensingLabel = QLabel("--")
            self.sensingLabel.setAlignment(Qt.AlignCenter)

            self.stabilizingTitle = QLabel("Stabilizing")
            self.stabilizingTitle.setAlignment(Qt.AlignCenter)

            self.stabilizingLabel = QLabel("--")
            self.stabilizingLabel.setAlignment(Qt.AlignCenter)

            self.reductionLabel = QLabel("--")
            self.reductionLabel.setAlignment(Qt.AlignCenter)

            # Sensing value
            self.sensingLabel.setStyleSheet(f"""
                color: {SECONDARY};
                font-size: 25pt;
                font-weight: bold;
            """)

            # Stabilizing value
            self.stabilizingLabel.setStyleSheet(f"""
                color: {PRIMARY};
                font-size: 25pt;
                font-weight: bold;
            """)

            # Reduction percentage
            self.reductionLabel.setStyleSheet("""
                color: black;
                font-size: 18pt;
                font-weight: bold;
            """)

            valueLayout.addWidget(self.sensingTitle)
            valueLayout.addWidget(self.sensingLabel)

            valueLayout.addWidget(self.stabilizingTitle)
            valueLayout.addWidget(self.stabilizingLabel)

            valueLayout.addWidget(self.reductionLabel)

        else:

            self.valueLabel = QLabel(f"0.00{valueUnit}")
            self.valueLabel.setAlignment(Qt.AlignCenter)

            valueFont = QFont("Arial")
            valueFont.setPointSize(20)
            valueFont.setBold(True)

            self.valueLabel.setFont(valueFont)

            self.valueLabel.setStyleSheet(f"""
                color: {SECONDARY};
                background: transparent;
            """)

            valueLayout.addWidget(self.valueLabel)

        self.plot.setBackground(None)

        self.plot.setTitle(
            title,
            color=TEXT,
            size="14pt",
            bold=True
        )

        self.plot.showGrid(x=True, y=True)

        self.plot.setYRange(yRange[0], yRange[1])

        # Axis labels
        self.plot.setLabel("left", units)
        self.plot.setLabel("bottom", "Time (s)")

        # Keep time-axis spacing consistent
        bottomAxis = self.plot.getAxis("bottom")
        bottomAxis.setTickSpacing(major=5, minor=1)

        self.plot.setMouseEnabled(False, False)

        self.plot.hideButtons()

        self.plot.setMenuEnabled(False)

        axisStyle = {
            "color": TEXT_SECONDARY,
            "font-size": "12pt",
            "font-weight": "bold"
        }

        self.plot.setLabel(
            "left",
            units,
            **axisStyle
        )

        self.plot.setLabel(
            "bottom",
            "Time (s)",
            **axisStyle
        )

        self.curves = []
        self.startNewCurve(SECONDARY)

        layout.addWidget(self.plot, 1)
        layout.addWidget(self.valueBox)

    # ========================================================
    # Update Graph
    # ========================================================

    def startNewCurve(self, colour):

        curve = self.plot.plot(
            pen=pg.mkPen(colour, width=2)
        )

        self.curves.append({
            "curve": curve,
            "time": [],
            "data": []
        })

    def updateValue(self, value):

        self.currentTime += 1 / self.sampleRate

        self.data.append(value)
        self.time.append(self.currentTime)

        segment = self.curves[-1]

        segment["time"].append(self.currentTime)
        segment["data"].append(value)

        segment["curve"].setData(
            segment["time"],
            segment["data"]
        )

        # Keep x-axis rolling
        windowSize = 20

        if self.currentTime <= windowSize:
            self.plot.setXRange(0, windowSize, padding=0)
        else:
            self.plot.setXRange(
                self.currentTime - windowSize,
                self.currentTime,
                padding=0
            )

        if not self.comparison:

            self.valueLabel.setText(
                f"{value:.2f}{self.valueUnit}"
            )

    def updateComparison(
        self,
        sensing,
        stabilizing=None,
        reduction=None
    ):

        if not self.comparison:
            return

        self.sensingLabel.setText(
            f"{sensing:.2f}{self.valueUnit}"
        )

        if stabilizing is not None:

            self.stabilizingLabel.setText(
                f"{stabilizing:.2f}{self.valueUnit}"
            )

        if reduction is not None:

            self.reductionLabel.setText(
                f"↓ {reduction:.0f}%"
            )

    # ========================================================
    # Clear
    # ========================================================

    def setMode(self, mode):

        self.mode = mode

        if mode == 0:
            colour = SECONDARY
        else:
            colour = PRIMARY

        self.startNewCurve(colour)

        if not self.comparison:
            self.valueLabel.setStyleSheet(
                f"color: {colour};"
            )
    def clear(self):

        # Clear stored data
        self.data.clear()
        self.time.clear()

        # Reset time to 0 seconds
        self.currentTime = 0.0
        self.mode = 0

        # Remove all existing curve segments
        for segment in self.curves:
            self.plot.removeItem(segment["curve"])

        self.curves.clear()

        # Start a fresh curve using the current mode colour
        if self.mode == 0:
            colour = SECONDARY
        else:
            colour = PRIMARY

        self.startNewCurve(colour)

        # Reset x-axis
        self.plot.setXRange(0, 20, padding=0)
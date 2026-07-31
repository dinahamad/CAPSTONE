"""
============================================================
TremorTech Graph Widgets
============================================================
"""

import pyqtgraph as pg

from PySide6.QtWidgets import QFrame
from PySide6.QtWidgets import QVBoxLayout

from styles import *

# ============================================================
# GRAPH WIDGET
# ============================================================

class OrientationGraph(QFrame):

    def __init__(self):

        super().__init__()

        self.setStyleSheet(CARD_STYLE)

        layout = QVBoxLayout(self)

        layout.setContentsMargins(15,15,15,15)

        self.graph = pg.PlotWidget()

        layout.addWidget(self.graph)

        self.setupGraph()

    # ========================================================

    def setupGraph(self):

        self.graph.setBackground(GRAPH_BACKGROUND)

        self.graph.setTitle(
            "Real-Time Wrist Orientation",
            color=PRIMARY,
            size="14pt"
        )

        self.graph.showGrid(
            x=True,
            y=True,
            alpha=0.25
        )

        self.graph.addLegend()

        self.graph.setLabel(
            "left",
            "Angle (°)"
        )

        self.graph.setLabel(
            "bottom",
            "Time (s)"
        )

        self.graph.getAxis("left").setTextPen(TEXT)

        self.graph.getAxis("bottom").setTextPen(TEXT)

        self.graph.getAxis("left").setPen(TEXT)

        self.graph.getAxis("bottom").setPen(TEXT)

        # Roll

        self.rollCurve = self.graph.plot(
            [],
            [],
            pen=pg.mkPen(
                PRIMARY,
                width=3
            ),
            name="Roll"
        )

        # Pitch

        self.pitchCurve = self.graph.plot(
            [],
            [],
            pen=pg.mkPen(
                SECONDARY,
                width=3
            ),
            name="Pitch"
        )

        # Yaw

        self.yawCurve = self.graph.plot(
            [],
            [],
            pen=pg.mkPen(
                "#2EC4B6",
                width=3
            ),
            name="Yaw"
        )

    # ========================================================

    def updateGraph(
        self,
        t,
        roll,
        pitch,
        yaw
    ):

        self.rollCurve.setData(
            t,
            roll
        )

        self.pitchCurve.setData(
            t,
            pitch
        )

        self.yawCurve.setData(
            t,
            yaw
        )


# ============================================================
# TREMOR GRAPH
# ============================================================

class TremorGraph(QFrame):

    def __init__(self):

        super().__init__()

        self.setStyleSheet(CARD_STYLE)

        layout = QVBoxLayout(self)

        layout.setContentsMargins(15,15,15,15)

        self.graph = pg.PlotWidget()

        layout.addWidget(self.graph)

        self.setupGraph()

    # ========================================================

    def setupGraph(self):

        self.graph.setBackground(GRAPH_BACKGROUND)

        self.graph.setTitle(
            "Tremor Frequency (3–12 Hz)",
            color=PRIMARY,
            size="14pt"
        )

        self.graph.showGrid(
            x=True,
            y=True,
            alpha=0.25
        )

        self.graph.setLabel(
            "left",
            "Frequency (Hz)"
        )

        self.graph.setLabel(
            "bottom",
            "Time (s)"
        )

        self.graph.getAxis("left").setTextPen(TEXT)

        self.graph.getAxis("bottom").setTextPen(TEXT)

        self.graph.getAxis("left").setPen(TEXT)

        self.graph.getAxis("bottom").setPen(TEXT)

        self.frequencyCurve = self.graph.plot(
            [],
            [],
            pen=pg.mkPen(
                PRIMARY,
                width=3
            )
        )

    # ========================================================

    def updateGraph(
        self,
        t,
        frequency
    ):

        self.frequencyCurve.setData(
            t,
            frequency
        )
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

from PySide6.QtWidgets import QWidget, QVBoxLayout

from styles import *


class LivePlot(QWidget):

    def __init__(
        self,
        title,
        colour,
        yRange=(-10, 10),
        history=1000,
        units=""
    ):

        super().__init__()

        self.history = history

        self.data = deque([0] * history, maxlen=history)

        layout = QVBoxLayout(self)

        layout.setContentsMargins(0, 0, 0, 0)

        # ----------------------------------------------------
        # Graph
        # ----------------------------------------------------

        self.plot = pg.PlotWidget()

        self.plot.setBackground(GRAPH_BACKGROUND)

        self.plot.setTitle(title)

        self.plot.showGrid(x=True, y=True)

        self.plot.setYRange(yRange[0], yRange[1])

        # Axis labels
        self.plot.setLabel("left", units)
        self.plot.setLabel("bottom", "Time (s)")

        self.plot.setMouseEnabled(False, False)

        self.plot.hideButtons()

        self.plot.setMenuEnabled(False)

        self.plot.getAxis("left").setPen(AXIS_COLOUR)

        self.plot.getAxis("bottom").setPen(AXIS_COLOUR)

        self.curve = self.plot.plot(
            pen=pg.mkPen(
                colour,
                width=2
            )
        )

        layout.addWidget(self.plot)

    # ========================================================
    # Update Graph
    # ========================================================

    def updateValue(self, value):

        self.data.append(value)

        self.curve.setData(self.data)

    # ========================================================
    # Clear
    # ========================================================

    def clear(self):

        self.data = deque(
            [0] * self.history,
            maxlen=self.history
        )

        self.curve.setData(self.data)
from PySide6.QtCore import Qt
from PySide6.QtWidgets import (
    QFrame,
    QLabel,
    QVBoxLayout,
    QPushButton,
    QGraphicsDropShadowEffect
)

from PySide6.QtGui import QColor

from styles import *


# ============================================================
# SHADOW
# ============================================================

def addShadow(widget):

    shadow = QGraphicsDropShadowEffect()

    shadow.setBlurRadius(20)

    shadow.setXOffset(0)

    shadow.setYOffset(4)

    shadow.setColor(QColor(0, 0, 0, 45))

    widget.setGraphicsEffect(shadow)


# ============================================================
# STATUS CARD
# ============================================================

class StatusCard(QFrame):

    def __init__(self, title):

        super().__init__()

        self.setStyleSheet(CARD_STYLE)

        addShadow(self)

        layout = QVBoxLayout(self)

        layout.setContentsMargins(15, 15, 15, 15)

        self.title = QLabel(title)

        self.title.setStyleSheet(LABEL_STYLE)

        self.status = QLabel("UNKNOWN")

        self.status.setAlignment(Qt.AlignCenter)

        self.status.setStyleSheet(status_style(WARNING))

        layout.addWidget(self.title)

        layout.addWidget(self.status)

    def setStatus(self, text):

        colour = STATUS.get(text.upper(), WARNING)

        self.status.setText(text.upper())

        self.status.setStyleSheet(
            status_style(colour)
        )


# ============================================================
# LIVE VALUE CARD
# ============================================================

class ValueCard(QFrame):

    def __init__(self, title, unit=""):

        super().__init__()

        self.unit = unit

        self.setStyleSheet(CARD_STYLE)

        addShadow(self)

        layout = QVBoxLayout(self)

        layout.setContentsMargins(15,15,15,15)

        self.title = QLabel(title)

        self.title.setStyleSheet(LABEL_STYLE)

        self.value = QLabel("--")

        self.value.setAlignment(Qt.AlignCenter)

        self.value.setStyleSheet(NUMBER_STYLE)

        layout.addWidget(self.title)

        layout.addWidget(self.value)

    def setValue(self, value):

        self.value.setText(
            f"{value}{self.unit}"
        )


# ============================================================
# BATTERY CARD
# ============================================================

class BatteryCard(QFrame):

    def __init__(self):

        super().__init__()

        self.setStyleSheet(CARD_STYLE)

        addShadow(self)

        layout = QVBoxLayout(self)

        layout.setContentsMargins(15,15,15,15)

        title = QLabel("Battery")

        title.setStyleSheet(LABEL_STYLE)

        self.value = QLabel("100%")

        self.value.setAlignment(Qt.AlignCenter)

        self.value.setStyleSheet(NUMBER_STYLE)

        layout.addWidget(title)

        layout.addWidget(self.value)

    def setBattery(self, percent):

        self.value.setText(f"{percent}%")

        if percent > 60:

            colour = SUCCESS

        elif percent > 25:

            colour = WARNING

        else:

            colour = ERROR

        self.value.setStyleSheet(f"""
        QLabel{{
            color:{colour};
            font-size:28px;
            font-weight:bold;
        }}
        """)


# ============================================================
# TREMOR CARD
# ============================================================

class TremorCard(QFrame):

    def __init__(self):

        super().__init__()

        self.setStyleSheet(CARD_STYLE)

        addShadow(self)

        layout = QVBoxLayout(self)

        layout.setContentsMargins(15,15,15,15)

        title = QLabel("Tremor Frequency")

        title.setStyleSheet(LABEL_STYLE)

        self.value = QLabel("-- Hz")

        self.value.setAlignment(Qt.AlignCenter)

        self.value.setStyleSheet(NUMBER_STYLE)

        self.severity = QLabel("UNKNOWN")

        self.severity.setAlignment(Qt.AlignCenter)

        layout.addWidget(title)

        layout.addWidget(self.value)

        layout.addWidget(self.severity)

    def setFrequency(self, hz):

        self.value.setText(
            f"{hz:.1f} Hz"
        )

        if hz < 3:

            level = "NONE"

            colour = SUCCESS

        elif hz < 6:

            level = "LOW"

            colour = SUCCESS

        elif hz < 9:

            level = "MODERATE"

            colour = WARNING

        else:

            level = "HIGH"

            colour = ERROR

        self.severity.setText(level)

        self.severity.setStyleSheet(f"""
        QLabel{{
            color:white;
            background:{colour};
            border-radius:8px;
            padding:6px;
            font-weight:bold;
        }}
        """)


# ============================================================
# MODERN BUTTON
# ============================================================

class DashboardButton(QPushButton):

    def __init__(self, text, colour=PRIMARY):

        super().__init__(text)

        addShadow(self)

        self.setStyleSheet(f"""

        QPushButton{{

            background:{colour};

            color:white;

            border:none;

            border-radius:10px;

            padding:12px;

            font-size:11pt;

            font-weight:bold;

        }}

        QPushButton:hover{{

            background:{SECONDARY};

        }}

        """)
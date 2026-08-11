# ============================================================
# Tremor Detection Dashboard
#
# File: widgets/header.py
#
# Description:
# Dashboard header containing:
# - Tremor Tech logo
# - Dashboard title
# - Subtitle
# - BLE connection indicator
# ============================================================

from PySide6.QtWidgets import (
    QWidget,
    QLabel,
    QHBoxLayout,
    QVBoxLayout,
    QFrame
)

from PySide6.QtGui import (
    QPixmap,
    QFont
)

from PySide6.QtCore import Qt

from styles import *


class Header(QWidget):

    def __init__(self):

        super().__init__()

        self.buildUI()

    # ========================================================
    # BUILD UI
    # ========================================================

    def buildUI(self):

        layout = QHBoxLayout(self)

        layout.setContentsMargins(10,10,10,10)

        layout.setSpacing(0)

        # ----------------------------------------------------
        # Logo
        # ----------------------------------------------------

        self.logo = QLabel()
        self.logo.setStyleSheet("""
            background: transparent;
            border: none;
        """)

        pixmap = QPixmap("assets/Tremor_Tech_Logo_blue.png")

        pixmap = pixmap.scaled(
            100,
            100,
            Qt.KeepAspectRatio,
            Qt.SmoothTransformation
        )

        self.logo.setPixmap(pixmap)

        layout.addWidget(self.logo)

        # ----------------------------------------------------
        # Title
        # ----------------------------------------------------

        titleLayout = QVBoxLayout()

        titleLayout.setContentsMargins(0, 0, 0, 0)
        titleLayout.setSpacing(0)

        titleLayout.setAlignment(Qt.AlignVCenter)


        self.title = QLabel(
            "Tremor Detection Dashboard"
        )

        titleFont = QFont()

        titleFont.setPointSize(30)

        titleFont.setBold(True)

        self.title.setFont(titleFont)

        self.title.setStyleSheet(f"""
        background: transparent;
        border: none;
        color: {PRIMARY};
        """)

        self.subtitle = QLabel(
            "Wearable Tremor Stabilization Device"
        )

        subtitleFont = QFont()

        subtitleFont.setPointSize(15)

        self.subtitle.setFont(subtitleFont)

        self.subtitle.setStyleSheet(f"""
        background: transparent;
        border: none;
        color: {TEXT_SECONDARY};
        """)

        titleLayout.addWidget(
            self.title,
            0,
            Qt.AlignBottom
        )

        titleLayout.addWidget(
            self.subtitle,
            0,
            Qt.AlignTop
        )

        layout.addLayout(titleLayout)

        layout.addStretch()

        # ----------------------------------------------------
        # Device Power
        # ----------------------------------------------------

        powerBox = QFrame()
        powerBox.setStyleSheet("background: transparent; border: none;")
        powerBox.setFixedWidth(150)

        powerLayout = QVBoxLayout(powerBox)
        powerLayout.setSpacing(5)

        powerTitle = QLabel("Device Power")
        powerTitle.setAlignment(Qt.AlignCenter)
        powerTitle.setStyleSheet("""
            font-size: 16pt;
            font-weight: bold;
        """)

        self.powerLabel = QLabel("OFF")
        self.powerLabel.setAlignment(Qt.AlignCenter)

        self.powerLabel.setStyleSheet(f"""
            color: {ERROR};
            font-size: 25pt;
            font-weight: bold;
        """)

        powerLayout.addWidget(powerTitle)
        powerLayout.addWidget(self.powerLabel)

        # ----------------------------------------------------
        # Current Mode
        # ----------------------------------------------------

        modeBox = QFrame()
        modeBox.setStyleSheet("background: transparent; border: none;")
        modeBox.setFixedWidth(220)

        modeLayout = QVBoxLayout(modeBox)
        modeLayout.setSpacing(5)

        modeTitle = QLabel("Current Mode")
        modeTitle.setAlignment(Qt.AlignCenter)
        modeTitle.setStyleSheet("""
            font-size: 16pt;
            font-weight: bold;
        """)

        self.modeLabel = QLabel("----------")
        self.modeLabel.setAlignment(Qt.AlignCenter)

        self.modeLabel.setStyleSheet(f"""
            color: {TEXT_SECONDARY};
            font-size: 25pt;
            font-weight: bold;
        """)

        modeLayout.addWidget(modeTitle)
        modeLayout.addWidget(self.modeLabel)

        # Add to top-right of header
        layout.addWidget(powerBox)
        layout.addSpacing(12)
        layout.addWidget(modeBox)


    # ========================================================
    # POWER
    # ========================================================

    def setPower(self, power):

        if power:
            self.powerLabel.setText("ON")
            self.powerLabel.setStyleSheet(
                f"color: {SUCCESS}; font-size: 25pt; font-weight: bold;"
            )

        else:
            self.powerLabel.setText("OFF")
            self.powerLabel.setStyleSheet(
                f"color: {ERROR}; font-size: 25pt; font-weight: bold;"
            )

            self.modeLabel.setText("----------")
            self.modeLabel.setStyleSheet(f"""
                color: {TEXT_SECONDARY};
                font-size: 25pt;
                font-weight: bold;
            """)

    # ========================================================
    # MODE
    # ========================================================

    def setMode(self, mode):

        if mode == 0:
            self.modeLabel.setText("SENSING")
            self.modeLabel.setStyleSheet(
                f"color: {SECONDARY}; font-size: 25pt; font-weight: bold;"
            )

        else:
            self.modeLabel.setText("STABILIZING")
            self.modeLabel.setStyleSheet(
                f"color: {PRIMARY}; font-size: 25pt; font-weight: bold;"
            )
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
    QVBoxLayout
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

        layout.setContentsMargins(20,20,20,20)

        layout.setSpacing(20)

        # ----------------------------------------------------
        # Logo
        # ----------------------------------------------------

        self.logo = QLabel()

        pixmap = QPixmap("assets/Tremor_Tech_Logo_blue.png")

        pixmap = pixmap.scaled(
            70,
            70,
            Qt.KeepAspectRatio,
            Qt.SmoothTransformation
        )

        self.logo.setPixmap(pixmap)

        layout.addWidget(self.logo)

        # ----------------------------------------------------
        # Title
        # ----------------------------------------------------

        titleLayout = QVBoxLayout()

        self.title = QLabel(
            "Tremor Detection Dashboard"
        )

        titleFont = QFont()

        titleFont.setPointSize(22)

        titleFont.setBold(True)

        self.title.setFont(titleFont)

        self.title.setStyleSheet(f"""
        background: transparent;
        border: none;
        color: {TEXT};
        """)

        self.subtitle = QLabel(
            "Wearable Tremor Stabilization Device"
        )

        subtitleFont = QFont()

        subtitleFont.setPointSize(11)

        self.subtitle.setFont(subtitleFont)

        self.subtitle.setStyleSheet(f"""
        background: transparent;
        border: none;
        color: {TEXT_SECONDARY};
        """)

        titleLayout.addWidget(self.title)

        titleLayout.addWidget(self.subtitle)

        layout.addLayout(titleLayout)

        layout.addStretch()

        # ----------------------------------------------------
        # BLE Status
        # ----------------------------------------------------

        self.connection = QLabel(
            "● Disconnected"
        )

        font = QFont()

        font.setPointSize(12)

        font.setBold(True)

        self.connection.setFont(font)

        self.connection.setStyleSheet(
            f"color:{ERROR};"
        )

        layout.addWidget(self.connection)

    # ========================================================
    # STATUS
    # ========================================================

    def setConnected(self, connected):

        if connected:

            self.connection.setText(
                "● Connected"
            )

            self.connection.setStyleSheet(
                f"color:{SUCCESS};"
            )

        else:

            self.connection.setText(
                "● Disconnected"
            )

            self.connection.setStyleSheet(
                f"color:{ERROR};"
            )
# ============================================================
# Tremor Detection Dashboard
#
# File: styles.py
#
# Description:
# Dashboard colours, fonts and stylesheet.
# ============================================================

# ============================================================
# COLOURS
# ============================================================

PRIMARY = "#3D2D76"
SECONDARY = "#789CEF"

BACKGROUND = "#EBF0FF"

CARD = "#FFFFFF"

TEXT = "#1F1F1F"

TEXT_SECONDARY = "#666666"

SUCCESS = "#43A047"

WARNING = "#FB8C00"

ERROR = "#E53935"

GRID = "#D6DCEE"

GRAPH_BACKGROUND = "#FFFFFF"

# ============================================================
# FONTS
# ============================================================

TITLE_FONT_SIZE = 22

HEADER_FONT_SIZE = 16

CARD_TITLE_SIZE = 15

CARD_VALUE_SIZE = 28

GRAPH_TITLE_SIZE = 13

# ============================================================
# MAIN WINDOW
# ============================================================

MAIN_STYLE = f"""
QMainWindow
{{
    background-color: {BACKGROUND};
}}

QWidget
{{
    background-color: {BACKGROUND};
    color: {TEXT};
    font-family: Arial;
}}

QLabel
{{
    color: {TEXT};
}}

QFrame
{{
    background: {CARD};
    border-radius: 15px;
}}

QPushButton
{{
    background-color: {PRIMARY};
    color: white;
    border-radius: 10px;
    padding: 8px;
    font-size: 12pt;
}}

QPushButton:hover
{{
    background-color: {SECONDARY};
}}
"""

# ============================================================
# VALUE CARD
# ============================================================

CARD_STYLE = f"""
QFrame
{{
    background: white;

    border: 1px solid #ECECEC;

    border-radius: 22px;
}}

QLabel
{{
    background: transparent;

    border: none;
}}
"""

# ============================================================
# STATUS COLOURS
# ============================================================

STATUS_COLOURS = {
    0: WARNING,
    1: SUCCESS
}

STATUS_TEXT = {
    0: "Monitoring",
    1: "Tremor Detected"
}

# ============================================================
# GRAPH COLOURS
# ============================================================

WRIST_COLOUR = "#3D2D76"

TREMOR_COLOUR = "#789CEF"

ENVELOPE_COLOUR = "#43A047"

FREQUENCY_COLOUR = "#FB8C00"

GRID_COLOUR = "#E0E0E0"

AXIS_COLOUR = "#555555"
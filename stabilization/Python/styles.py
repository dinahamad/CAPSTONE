"""
============================================================
TremorTech Dashboard Theme
============================================================
"""

# ============================================================
# BRAND COLOURS
# ============================================================

PRIMARY = "#3d2d76"       # Main Purple (logo)
SECONDARY = "#789cef"     # Blue (logo)
ACCENT = "#ebf0ff"        # Very light blue

# ============================================================
# UI COLOURS
# ============================================================

BACKGROUND = "#ebf0ff"
CARD = "#FFFFFF"

TEXT = "#2E2E2E"
SUBTEXT = "#707070"

SUCCESS = "#43A047"
WARNING = "#FB8C00"
ERROR = "#E53935"

BORDER = "#D9DCE5"

GRAPH_BACKGROUND = "#FFFFFF"

# ============================================================
# OPTIONAL EXTRA COLOURS
# ============================================================

TITLE = PRIMARY
BUTTON_TEXT = "#FFFFFF"

CARD_HOVER = "#F8F9FD"

GRID = "#E8EBF3"

ROLL = "#789cef"
PITCH = "#43A047"
YAW = PRIMARY


# ------------------------------------------------------------
# Global Qt Style Sheet
# ------------------------------------------------------------

STYLE_SHEET = f"""

QMainWindow {{
    background-color:{BACKGROUND};
}}

QWidget {{
    background-color:{BACKGROUND};
    color:{TEXT};
    font-family:'Segoe UI';
    font-size:11pt;
}}

QLabel {{
    background:transparent;
}}

QPushButton {{

    background-color:{PRIMARY};

    color:white;

    border:none;

    border-radius:10px;

    padding:10px;

    min-height:18px;

    font-size:11pt;

    font-weight:bold;

}}

QPushButton:hover {{

    background-color:#5e1789;

}}

QPushButton:pressed {{

    background-color:#32024d;

}}

QFrame {{

    background-color:{CARD};

    border:1px solid {BORDER};

    border-radius:15px;

}}

QGroupBox {{

    border:1px solid {BORDER};

    border-radius:15px;

    margin-top:10px;

    font-weight:bold;

    background:white;

}}

QGroupBox:title {{

    subcontrol-origin: margin;

    left:12px;

    padding:0 5px;

}}

QStatusBar {{

    background:white;

}}

"""


# ------------------------------------------------------------
# Status Card Colours
# ------------------------------------------------------------

STATUS = {

    "CONNECTED": SUCCESS,
    "READY": SUCCESS,
    "ACTIVE": SUCCESS,
    "WORKING": SUCCESS,
    "OK": SUCCESS,
    "CALIBRATED": SUCCESS,

    "WARNING": WARNING,

    "ERROR": ERROR,
    "FAULT": ERROR,
    "FAILED": ERROR,
    "DISCONNECTED": ERROR,
    "NOT CONNECTED": ERROR,
    "NOT CALIBRATED": ERROR

}


# ------------------------------------------------------------
# Card Style Helper
# ------------------------------------------------------------

CARD_STYLE = f"""
QFrame {{
    background: {CARD};
    border: none;
    border-radius: 18px;
}}
"""


# ------------------------------------------------------------
# Title Styles
# ------------------------------------------------------------

TITLE_STYLE = f"""

QLabel{{
    color:{PRIMARY};
    font-size:28px;
    font-weight:700;
}}

"""

SUBTITLE_STYLE = f"""

QLabel{{
    color:{SUBTEXT};
    font-size:12pt;
}}

"""


# ------------------------------------------------------------
# Large Number Style
# ------------------------------------------------------------

NUMBER_STYLE = f"""

QLabel{{
    color:{PRIMARY};
    font-size:30px;
    font-weight:bold;
}}

"""


# ------------------------------------------------------------
# Small Label Style
# ------------------------------------------------------------

LABEL_STYLE = f"""

QLabel{{
    color:{SUBTEXT};
    font-size:10pt;
}}

"""


# ------------------------------------------------------------
# Status Card Generator
# ------------------------------------------------------------

def status_style(colour):

    return f"""

    QLabel{{

        background:{colour};

        color:white;

        border-radius:12px;

        padding:10px;

        font-weight:bold;

        font-size:11pt;

    }}

    """
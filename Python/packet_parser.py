# ============================================================
# Tremor Detection Dashboard
#
# File: packet_parser.py
#
# Description:
# Parses packets received from the ESP32.
#
# Expected packet format:
#
# TREMOR,
# timestamp,
# handPitch,
# forearmPitch,
# wristPitch,
# tremorPitch,
# envelope,
# frequency,
# confidence,
# state
# ============================================================

from constants import *


class PacketParser:

    @staticmethod
    def parse(packet: str):

        packet = packet.strip()

        if not packet.startswith(PACKET_IDENTIFIER):
            return None

        parts = packet.split(",")

        if len(parts) != 10:
            return None

        try:

            data = {

                "timestamp":
                    int(parts[1]),

                "handPitch":
                    float(parts[2]),

                "forearmPitch":
                    float(parts[3]),

                "wristPitch":
                    float(parts[4]),

                "tremorPitch":
                    float(parts[5]),

                "envelope":
                    float(parts[6]),

                "frequency":
                    float(parts[7]),

                "confidence":
                    float(parts[8]),

                "detection":
                    int(parts[9])

            }

            return data

        except ValueError:

            return None
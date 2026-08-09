# ============================================================
# Tremor Detection Dashboard
#
# File: serial_manager.py
#
# Description:
# Reads live tremor packets from the ESP32 over USB Serial
# and forwards them to the dashboard.
# ============================================================

import serial
import serial.tools.list_ports

from PySide6.QtCore import QObject, Signal, QThread

from packet_parser import PacketParser


class SerialManager(QThread):

    # Emitted whenever a valid packet is received
    dataReceived = Signal(dict)

    # True = connected
    # False = disconnected
    connectionChanged = Signal(bool)

    def __init__(self):

        super().__init__()

        self.serial = None
        self.running = True

    # ============================================================
    # Automatically find the ESP32 Serial Port
    # ============================================================

    def findPort(self):

        ports = serial.tools.list_ports.comports()

        for port in ports:

            description = port.description.lower()

            if (
                "usb" in description or
                "cp210" in description or
                "ch340" in description or
                "uart" in description or
                "serial" in description
            ):
                return port.device

        return None

    # ============================================================
    # Main Thread
    # ============================================================

    def run(self):

        port = self.findPort()

        if port is None:

            print("No serial device found.")

            self.connectionChanged.emit(False)

            return

        try:

            self.serial = serial.Serial(

                port=port,
                baudrate=115200,
                timeout=1

            )

            print(f"Connected to {port}")

            self.connectionChanged.emit(True)

        except Exception as e:

            print(e)

            self.connectionChanged.emit(False)

            return

        while self.running:

            try:

                line = self.serial.readline()

                if not line:

                    continue

                packet = line.decode(
                    "utf-8",
                    errors="ignore"
                ).strip()

                parsed = PacketParser.parse(packet)

                if parsed is not None:

                    self.dataReceived.emit(parsed)

            except Exception as e:

                print(e)

                break

        self.connectionChanged.emit(False)

    # ============================================================
    # Disconnect
    # ============================================================

    def stop(self):

        self.running = False

        if self.serial is not None:

            self.serial.close()
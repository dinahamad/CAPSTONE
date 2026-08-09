# ============================================================
# Tremor Detection Dashboard
#
# File: ble_manager.py
#
# Description:
# BLE communication with ESP32.
# ============================================================

import asyncio

from bleak import BleakClient
from bleak import BleakScanner

from PySide6.QtCore import QObject
from PySide6.QtCore import Signal

from constants import *

from packet_parser import PacketParser


class BLEManager(QObject):

    dataReceived = Signal(dict)

    connectionChanged = Signal(bool)

    def __init__(self, dashboard):

        super().__init__()

        self.dashboard = dashboard

        self.client = None

        self.connected = False

    # ========================================================
    # CONNECT
    # ========================================================

    async def connect(self):

        print("Searching for ESP32...")

        devices = await BleakScanner.discover()

        for device in devices:

            if device.name is None:
                continue

            if DEVICE_NAME in device.name:

                print("Found:", device.name)

                self.client = BleakClient(device)

                await self.client.connect()

                self.connected = True

                self.connectionChanged.emit(True)

                print("Connected!")

                await self.client.start_notify(

                    DATA_CHARACTERISTIC_UUID,

                    self.notificationHandler

                )

                return True

        print("ESP32 not found.")

        return False

    # ========================================================
    # BLE Notification Callback
    # ========================================================

    def notificationHandler(self, sender, data):
        """
        Called automatically whenever the ESP32 sends a BLE notification.
        """

        try:

            packet = data.decode("utf-8")

        except Exception:

            return

        parsed = PacketParser.parse(packet)

        if parsed is None:
            return

        self.dataReceived.emit(parsed)
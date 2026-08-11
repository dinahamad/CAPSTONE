# ============================================================
# Tremor Detection Dashboard
# File: ble_manager.py
# Description: Receives dashboard packets from ESP32 over BLE.
# ============================================================

import asyncio
from bleak import BleakClient, BleakScanner
from PySide6.QtCore import QThread, Signal

from constants import DEVICE_NAME, DATA_CHARACTERISTIC_UUID
from packet_parser import PacketParser


class BLEManager(QThread):
    dataReceived = Signal(dict)
    connectionChanged = Signal(bool)

    def __init__(self):
        super().__init__()
        self.running = True
        self.client = None
        self.loop = None

    def notificationHandler(self, sender, data):
        try:
            text = data.decode("utf-8", errors="ignore")

            # DEBUG: show every packet received over BLE
            print(f"BLE RECEIVED: {repr(text)}")

            # One BLE notification may contain one or more newline packets.
            for line in text.splitlines():

                line = line.strip()

                if not line:
                    continue

                print(f"BLE PACKET: {line}")

                parsed = PacketParser.parse(line)

                if parsed is not None:
                    print(f"PARSED: {parsed}")
                    self.dataReceived.emit(parsed)

        except Exception as e:
            print(f"BLE packet error: {e}")

    async def connectAndListen(self):
        while self.running:
            try:
                print(f"Searching for {DEVICE_NAME}...")
                device = await BleakScanner.find_device_by_name(
                    DEVICE_NAME,
                    timeout=5.0
                )

                if device is None:
                    self.connectionChanged.emit(False)
                    await asyncio.sleep(2)
                    continue

                self.client = BleakClient(device)
                await self.client.connect()

                print(f"BLE connected to {DEVICE_NAME}")
                self.connectionChanged.emit(True)

                await self.client.start_notify(
                    DATA_CHARACTERISTIC_UUID,
                    self.notificationHandler
                )

                while self.running and self.client.is_connected:
                    await asyncio.sleep(0.25)

            except Exception as e:
                print(f"BLE error: {e}")

            finally:
                self.connectionChanged.emit(False)
                if self.client is not None:
                    try:
                        if self.client.is_connected:
                            await self.client.disconnect()
                    except Exception:
                        pass
                self.client = None

            if self.running:
                await asyncio.sleep(1)

    def run(self):
        self.loop = asyncio.new_event_loop()
        asyncio.set_event_loop(self.loop)
        self.loop.run_until_complete(self.connectAndListen())
        self.loop.close()

    def stop(self):
        self.running = False
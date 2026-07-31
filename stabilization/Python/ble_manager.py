import asyncio
import threading

from bleak import BleakScanner
from bleak import BleakClient

from PySide6.QtCore import QObject, Signal


# ============================================================
# BLE SETTINGS
# ============================================================

DEVICE_NAME = "Tremor Stabilization Glove"

SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

DATA_CHAR_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

COMMAND_CHAR_UUID = "6e400002-b5a3-f393-e0a9-e50e24dcca9e"

STATUS_CHAR_UUID = "6e400003-b5a3-f393-e0a9-e50e24dcca9e"


# ============================================================
# SIGNALS
# ============================================================

class DashboardSignals(QObject):

    connectionChanged = Signal(bool)

    statusReceived = Signal(str)

    dataReceived = Signal(str)

    errorReceived = Signal(str)

    batteryReceived = Signal(int)


# ============================================================
# BLE MANAGER
# ============================================================

class BLEManager:

    def __init__(self, signals):

        self.signals = signals

        self.client = None

        self.loop = None

        self.running = True

        self.connected = False

        self.packetCount = 0

    # ========================================================

    def start(self):

        thread = threading.Thread(
            target=self.runBLELoop,
            daemon=True
        )

        thread.start()

    # ========================================================

    def stop(self):

        self.running = False

    # ========================================================

    def runBLELoop(self):

        self.loop = asyncio.new_event_loop()

        asyncio.set_event_loop(self.loop)

        self.loop.run_until_complete(
            self.bleMain()
        )

    # ========================================================

    async def bleMain(self):

        while self.running:

            try:

                self.signals.statusReceived.emit(
                    "Scanning for TremorTech device..."
                )

                device = await BleakScanner.find_device_by_name(
                    DEVICE_NAME,
                    timeout=5
                )

                if device is None:

                    self.connected = False

                    self.signals.connectionChanged.emit(False)

                    self.signals.statusReceived.emit(
                        "Device not found"
                    )

                    await asyncio.sleep(2)

                    continue

                self.signals.statusReceived.emit(
                    "Connecting..."
                )

                async with BleakClient(device) as client:

                    self.client = client

                    self.connected = True

                    self.signals.connectionChanged.emit(True)

                    self.signals.statusReceived.emit(
                        "Connected"
                    )

                    await client.start_notify(
                        DATA_CHAR_UUID,
                        self.dataCallback
                    )

                    await client.start_notify(
                        STATUS_CHAR_UUID,
                        self.statusCallback
                    )

                    await client.write_gatt_char(
                        COMMAND_CHAR_UUID,
                        b"STATUS"
                    )

                    while client.is_connected:

                        await asyncio.sleep(1)

            except Exception as error:

                self.connected = False

                self.client = None

                self.signals.connectionChanged.emit(False)

                self.signals.errorReceived.emit(
                    str(error)
                )

                self.signals.statusReceived.emit(
                    "Disconnected"
                )

                await asyncio.sleep(2)

    # ========================================================

    def dataCallback(self, sender, data):

        try:

            message = data.decode("utf-8")

            self.packetCount += 1

            if message.startswith("BATTERY,"):

                try:

                    percent = int(
                        message.split(",")[1]
                    )

                    self.signals.batteryReceived.emit(
                        percent
                    )

                except:

                    pass

            else:

                self.signals.dataReceived.emit(
                    message
                )

        except Exception as error:

            self.signals.errorReceived.emit(
                str(error)
            )

    # ========================================================

    def statusCallback(self, sender, data):

        try:

            message = data.decode("utf-8")

            self.signals.statusReceived.emit(
                message
            )

        except Exception as error:

            self.signals.errorReceived.emit(
                str(error)
            )

    # ========================================================

    def sendCommand(self, command):

        if (
            self.loop is None
            or self.client is None
            or not self.client.is_connected
        ):
            return

        asyncio.run_coroutine_threadsafe(

            self.client.write_gatt_char(
                COMMAND_CHAR_UUID,
                command.encode()
            ),

            self.loop

        )
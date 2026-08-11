#include "ble_manager.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

namespace
{
    const char *DEVICE_NAME = "Tremor Stabilization Glove";
    const char *SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
    const char *DATA_CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";

    BLECharacteristic *dataCharacteristic = nullptr;
    bool deviceConnected = false;

    class ServerCallbacks : public BLEServerCallbacks
    {
        void onConnect(BLEServer *server) override
        {
            deviceConnected = true;
        }

        void onDisconnect(BLEServer *server) override
        {
            deviceConnected = false;
            BLEDevice::startAdvertising();
        }
    };
}

void BLEManager_init()
{
    BLEDevice::init(DEVICE_NAME);

    BLEServer *server = BLEDevice::createServer();
    server->setCallbacks(new ServerCallbacks());

    BLEService *service = server->createService(SERVICE_UUID);

    dataCharacteristic = service->createCharacteristic(
        DATA_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY
    );

    dataCharacteristic->addDescriptor(new BLE2902());

    service->start();

    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(SERVICE_UUID);
    advertising->setScanResponse(true);
    advertising->start();

    Serial.println("BLE dashboard ready");
}

void BLEManager_send(const String &packet)
{
    if (!deviceConnected || dataCharacteristic == nullptr)
        return;

    dataCharacteristic->setValue(packet.c_str());
    dataCharacteristic->notify();
}

bool BLEManager_isConnected()
{
    return deviceConnected;
}
#pragma once

#include <Arduino.h>

void BLEManager_init();
void BLEManager_send(const String &packet);
bool BLEManager_isConnected();
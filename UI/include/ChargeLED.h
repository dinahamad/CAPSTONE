#ifndef CHARGELED_H
#define CHARGELED_H

void ChargeLED_init();
void LEDsoff();
void LowPower_flash();

void ChargeLED_update(float batteryPercent, bool usbConnected);

#endif
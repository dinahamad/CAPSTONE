#ifndef CHARGELED_H
#define CHARGELED_H

void ChargeLED_init();
void LEDsoff();

void ChargeLED_update(float batteryPercent, bool usbConnected);

#endif
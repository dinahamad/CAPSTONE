#ifndef BATTERY_H
#define BATTERY_H

void Battery_init();
float Battery_getVoltage();
float Battery_getPercentage();
bool Check_Battery_isLow();
void Battery_printStatus();

#endif


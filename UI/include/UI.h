#ifndef UI_H
#define UI_H

void USB_detect_init();
bool USB_connected();

void Button_waitForRelease();
bool validButtonPress();
void Button_init();

void goToLightSleep();

#endif
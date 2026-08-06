#ifndef UI_H
#define UI_H

void USB_detect_init();
bool USB_connected();

void Button_waitForRelease();
bool validButtonPress(uint8_t BUTTON_NUMBER);
void Button_init();

void Wake_init();

void goToLightSleep();
void changeStableState();

#endif
#ifndef UI_H
#define UI_H

enum ButtonPress {
    NO_PRESS,
    SHORT_PRESS,
    LONG_PRESS
};

ButtonPress getStableButtonPress();

void USB_detect_init();
bool USB_connected();

void Button_waitForRelease();
unsigned long validButtonPress(uint8_t BUTTON_NUMBER);
void Button_init();

void Wake_init();

void goToLightSleep();
void changeStableState();

#endif

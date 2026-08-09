#ifndef UARTLINK_H
#define UARTLINK_H
#include "State.h"

void UARTLink_init();
void UARTLink_receive();
void shutdownBeforeSleep();
void goToLightSleep();
void UARTLink_update();

#endif
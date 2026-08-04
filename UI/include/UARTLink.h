#ifndef UARTLINK_H
#define UARTLINK_H
#include "State.h"

void UARTLink_init();
bool UARTLink_sendState(State state);

#endif
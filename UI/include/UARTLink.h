#ifndef UARTLINK_H
#define UARTLINK_H
#include "State.h"

void UARTLink_init();
bool UARTLink_sendState(systemState);
bool UARTLink_sendState(stableState);

#endif

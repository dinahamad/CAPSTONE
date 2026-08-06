#ifndef UARTLINK_H
#define UARTLINK_H
#include "State.h"

void UARTLink_init();
bool UARTLink_sendState(SystemState state);
bool UARTLink_sendState(StableState state);
void UARTLink_receive();

float UARTLink_getSlaveBattery();


#endif
#ifndef STATE_H
#define STATE_H

enum State {
    LIGHT_SLEEP,
    NORMAL
};

// This tells other files the variable exists
extern State systemState;

#endif
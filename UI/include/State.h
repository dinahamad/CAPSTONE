#ifndef STATE_H
#define STATE_H

enum SystemState {
    LIGHT_SLEEP,
    AWAKE
};

enum StableState {
    SENSE,
    STABILIZE
};

extern SystemState systemState;
extern StableState stableState;

#endif
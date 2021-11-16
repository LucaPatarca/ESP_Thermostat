#pragma once

#include <temperature.h>

enum Mode{
    OFF,
    ON,
    PROGRAM
};

class StateListener{
    public:
        virtual void onTargetTemperature(float) = 0;
        virtual void onPowerState(bool) = 0;
        virtual void onThermostatMode(Mode) = 0;
};
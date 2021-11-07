#pragma once

#include <temperature.h>

class StateListener{
    public:
        virtual void onTargetTemperature(float) = 0;
        virtual void onPowerState(bool) = 0;
};
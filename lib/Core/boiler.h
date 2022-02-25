#pragma once

#include <state.h>

enum TCase
{
    OFF_ABOVE_SAFE_RANGE,
    OFF_BELOW_SAFE_RANGE,
    OFF_IN_SAFE_RANGE,
    BELOW_LOW_RANGE,
    BELOW_RANGE_HIGH_RISING,
    BELOW_RANGE_HIGH_NOT_RISING,
    BELOW_TARGET_DROPPING,
    BELOW_TARGET_NOT_DROPPING,
    ABOVE_TARGET
};

class BoilerController
{
private:
    State &m_state;
    uint8_t m_toSet;
    unsigned int m_lastChange;

    TCase getTempCase();
    void compute();
    void setBoilerState(bool);

    BoilerController();

public:
    BoilerController(const BoilerController&) = delete;

    static BoilerController& Instance(){
        static BoilerController controller;
        return controller;
    }

    void currentTemperatureChanged();
    void powerStateChanged();
    void targetTemperatureChanged();

    void handle();
};
#pragma once

#include <temperature.h>
#include <TemperatureListener.h>
#include <StateListener.h>
#include <EventEmitter.h>
#include <BoilerListener.h>

#define TEMP_RANGE_LOW 1
#define TEMP_RANGE_HIGH 0.3
#define SAFE_TEMP 15

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

//TODO it may be useful to make this class an UpdateListener so that it can turn off the boiler during updates

class BoilerController : public EventEmitter<BoilerListener>, public TemperatureListener, public StateListener
{
private:
    bool _boilerState;
    Temperature _temperature;
    float _targetTemperature;
    bool _powerState;

    void setBoilerState(bool);
    TCase getTempCase();
    void compute();

public:
    void onCurrentTemperature(Temperature_t) override;
    void onPowerState(bool) override;
    void onThermostatMode(Mode) override;
    void onTargetTemperature(float) override;
};
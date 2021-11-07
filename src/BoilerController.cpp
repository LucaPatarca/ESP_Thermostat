#include <BoilerController.h>

void BoilerController::onCurrentTemperature(Temperature_t temp){
    _temperature = temp;
    compute();
}

void BoilerController::onPowerState(bool state){
    _powerState = state;
    compute();
}

void BoilerController::onTargetTemperature(float temp){
    _targetTemperature = temp;
    compute();
}


void BoilerController::setBoilerState(bool state)
{
    if (state != _boilerState)
    {
        _boilerState = state;
        for(BoilerListener *listener: _listeners){
            listener->onBoilerState(_boilerState);
        }
    }
}

void BoilerController::compute()
{
    TCase tempCase = getTempCase();
    switch (tempCase)
    {
    case TCase::OFF_BELOW_SAFE_RANGE:
    case TCase::BELOW_LOW_RANGE:
    case TCase::BELOW_RANGE_HIGH_NOT_RISING:
    case TCase::BELOW_TARGET_DROPPING:
        setBoilerState(true);
        break;

    case TCase::OFF_ABOVE_SAFE_RANGE:
    case TCase::BELOW_RANGE_HIGH_RISING:
    case TCase::BELOW_TARGET_NOT_DROPPING:
    case TCase::ABOVE_TARGET:
        setBoilerState(false);
        break;

    default:
        break;
    }
}

TCase BoilerController::getTempCase()
{
    if (!_powerState)
    {
        if (_temperature.temp < SAFE_TEMP)
        {
            return TCase::OFF_BELOW_SAFE_RANGE;
        }
        else if (_temperature.temp > SAFE_TEMP + TEMP_RANGE_LOW)
        {
            return TCase::OFF_ABOVE_SAFE_RANGE;
        }
        else
        {
            return TCase::OFF_IN_SAFE_RANGE;
        }
    }
    else if (_temperature.temp < _targetTemperature - TEMP_RANGE_LOW)
    {
        return TCase::BELOW_LOW_RANGE;
    }
    else if (_temperature.temp < _targetTemperature - TEMP_RANGE_HIGH)
    {
        if (_temperature.trend == TemperatureTrend::RISE)
        {
            return TCase::BELOW_RANGE_HIGH_RISING;
        }
        else
        {
            return TCase::BELOW_RANGE_HIGH_NOT_RISING;
        }
    }
    else if (_temperature.temp < _targetTemperature)
    {
        if (_temperature.trend == TemperatureTrend::DROP)
        {
            return TCase::BELOW_TARGET_DROPPING;
        }
        else
        {
            return TCase::BELOW_TARGET_NOT_DROPPING;
        }
    }
    else
    {
        return TCase::ABOVE_TARGET;
    }
}
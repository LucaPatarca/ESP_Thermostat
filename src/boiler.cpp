#include <boiler.h>
#include <sdebug.h>

#define TEMP_RANGE_LOW 1
#define TEMP_RANGE_HIGH 0.3
#define SAFE_TEMP 15

BoilerController::BoilerController()
    : m_state(State::Instance())
{
}

void BoilerController::currentTemperatureChanged()
{
    compute();
}

void BoilerController::powerStateChanged()
{
    compute();
}

void BoilerController::targetTemperatureChanged()
{
    compute();
}

void BoilerController::compute()
{
    FINE("computing...");
    TCase tempCase = getTempCase();
    switch (tempCase)
    {
    case TCase::OFF_BELOW_SAFE_RANGE:
    case TCase::BELOW_LOW_RANGE:
    case TCase::BELOW_RANGE_HIGH_NOT_RISING:
    case TCase::BELOW_TARGET_DROPPING:
        INFO("setting boiler on");
        m_state.setBoilerState(true);
        break;

    case TCase::OFF_ABOVE_SAFE_RANGE:
    case TCase::BELOW_RANGE_HIGH_RISING:
    case TCase::BELOW_TARGET_NOT_DROPPING:
    case TCase::ABOVE_TARGET:
        INFO("setting boiler off");
        m_state.setBoilerState(false);
        break;

    default:
        break;
    }
}

TCase BoilerController::getTempCase()
{
    if (!m_state.getPowerState())
    {
        if (m_state.getCurrentTemperature().temp < SAFE_TEMP)
        {
            return TCase::OFF_BELOW_SAFE_RANGE;
        }
        else if (m_state.getCurrentTemperature().temp > SAFE_TEMP + TEMP_RANGE_LOW)
        {
            return TCase::OFF_ABOVE_SAFE_RANGE;
        }
        else
        {
            return TCase::OFF_IN_SAFE_RANGE;
        }
    }
    else if (m_state.getCurrentTemperature().temp < m_state.getTargetTemperature() - TEMP_RANGE_LOW)
    {
        return TCase::BELOW_LOW_RANGE;
    }
    else if (m_state.getCurrentTemperature().temp < m_state.getTargetTemperature() - TEMP_RANGE_HIGH)
    {
        if (m_state.getCurrentTemperature().trend == TemperatureTrend::RISE)
        {
            return TCase::BELOW_RANGE_HIGH_RISING;
        }
        else
        {
            return TCase::BELOW_RANGE_HIGH_NOT_RISING;
        }
    }
    else if (m_state.getCurrentTemperature().temp < m_state.getTargetTemperature())
    {
        if (m_state.getCurrentTemperature().trend == TemperatureTrend::DROP)
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
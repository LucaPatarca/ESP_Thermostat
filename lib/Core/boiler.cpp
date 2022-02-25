#include <boiler.h>
#include <sdebug.h>
#include <logger.h>

#define TEMP_RANGE_LOW 1
#define TEMP_RANGE_HIGH 0.3
#define SAFE_TEMP 15
#define BOILER_PIN D5
#define ON 1
#define OFF 2
#define NONE 3
#define CHANGE_DELAY 3000 // in milliseconds

BoilerController::BoilerController()
    : m_state(State::Instance()),
      m_lastChange(300000)
{
    pinMode(BOILER_PIN, OUTPUT);
    digitalWrite(BOILER_PIN, LOW);
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

void BoilerController::setBoilerState(bool value)
{
    m_toSet = value ? ON : OFF;
    if (millis() > m_lastChange)
    {
        m_lastChange = millis();
    }
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
    SEND_REMOTE_LOG;
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

void BoilerController::handle()
{
    if (m_toSet != NONE && millis() > m_lastChange + CHANGE_DELAY)
    {
        INFO("setting boiler %s", m_toSet == ON ? "on" : "off");
        m_state.setBoilerState(m_toSet == ON);
        digitalWrite(BOILER_PIN, m_toSet == ON ? HIGH : LOW);
        m_toSet = NONE;
    }
}
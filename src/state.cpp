#include <state.h>

#define UTC_OFFSET 3600
#define UPDATE_INTERVAL 1800000  //update every 30 minutes

State::State()
    : m_boilerState(false),
    m_targetTemperature(0.0),
    m_powerState(false),
    m_thermostatMode(Mode::OFF),
    m_currentTemperature(Temperature_t{0,0,TemperatureTrend::STABLE,0}),
    m_wifiStatus(WiFiStatus::DISCONNECTED),
    m_client(NTPClient(m_udp, "pool.ntp.org", UTC_OFFSET, UPDATE_INTERVAL))
{
    m_client.begin();
}

void State::setBoilerState(bool state)
{
    m_boilerState = state;
    m_listener->boilerStateChanged();
}

void State::setTargetTemperature(Cause cause, float temp)
{
    m_targetTemperature = temp;
    m_listener->targetTemperatureChanged(cause);
    setThermostatMode(Cause::AUTO, Mode::ON);
}

void State::setPowerState(Cause cause, bool state)
{
    m_powerState = state;
    m_listener->powerStateChanged(cause);

    if (cause != Cause::SCHEDULE)
    {
        m_thermostatMode = state ? Mode::ON : Mode::OFF;
        m_listener->thermostatModeChanged(Cause::AUTO);
    }
}

void State::setThermostatMode(Cause cause, Mode mode)
{
    m_thermostatMode = mode;
    m_listener->thermostatModeChanged(cause);
    if (mode != Mode::PROGRAM)
    {
        m_powerState = mode == Mode::ON;
        m_listener->powerStateChanged(Cause::AUTO);
    }
}

void State::setCurrentTemperature(Temperature_t &temp)
{
    m_currentTemperature = temp;
    m_listener->currentTemperatureChanged();
}

void State::setwWifiStatus(WiFiStatus status)
{
    m_wifiStatus = status;
    m_listener->wifiStatusChanged();
}

bool State::getBoilerState() const
{
    return m_boilerState;
}

float State::getTargetTemperature() const
{
    return m_targetTemperature;
}

bool State::getPowerState() const
{
    return m_powerState;
}

Mode State::getThermostatMode() const
{
    return m_thermostatMode;
}

const Temperature_t& State::getCurrentTemperature() const
{
    return m_currentTemperature;
}

WiFiStatus State::getWifiStatus() const
{
    return m_wifiStatus;
}


Time_t State::getTime()
{
    m_client.update();
    int day = (m_client.getDay() + 6) % 7;
    int hour = m_client.getHours();
    int min = m_client.getMinutes();
    return Time_t{hour, min, day};
}

String State::getFormattedTime()
{
    return m_client.getFormattedTime();
}
    
void State::addListener(StateListener* listener)
{
    m_listener = listener;
}
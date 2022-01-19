#include <state.h>
#include <sdebug.h>

#if LOG_LEVEL > LOG_LEVEL_OFF
const char *thermostatModeNames[] = {"OFF", "ON", "PROGRAM"};
const char *tempTrendNames[] = {"STABLE", "DROP", "RISE"};
const char *wifiStatusNames[] = {"DISCONNECTED", "CONNECTED", "CONNECTING"};
#endif

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
    if(state == m_boilerState) return;
    m_boilerState = state;
    INFO("boiler is now %s", state?"on":"off");
    m_listener->boilerStateChanged();
}

void State::setTargetTemperature(Cause cause, float temp)
{
    if(temp == m_targetTemperature) return;
    m_targetTemperature = temp;
    INFO("target temp is now %.1f", temp);
    m_listener->targetTemperatureChanged(cause);
    if(cause != Cause::SCHEDULE)
        setThermostatMode(Cause::AUTO, Mode::ON);
}

void State::setPowerState(Cause cause, bool state)
{
    if(state == m_powerState) return;
    m_powerState = state;
    INFO("power state is now %s", state?"on":"off");
    m_listener->powerStateChanged(cause);

    if (cause != Cause::SCHEDULE)
    {
        setThermostatMode(Cause::AUTO, state ? Mode::ON : Mode::OFF);
    }
}

void State::setThermostatMode(Cause cause, Mode mode)
{
    if(mode == m_thermostatMode) return;
    m_thermostatMode = mode;
    INFO("thermostat mode is now %s", thermostatModeNames[mode]);
    m_listener->thermostatModeChanged(cause);
    if (mode != Mode::PROGRAM)
    {
        setPowerState(Cause::AUTO, mode == Mode::ON);
    }
}

void State::setCurrentTemperature(const Temperature_t &temp)
{
    if(temp == m_currentTemperature) return;
    INFO("current temperature is now %.1f", temp.temp);
    m_currentTemperature = temp;
    m_listener->currentTemperatureChanged();
}

void State::setwWifiStatus(WiFiStatus status)
{
    if(status == m_wifiStatus) return;
    INFO("wifi status is now %s", wifiStatusNames[status]);
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

void State::reset()
{
    m_boilerState = false;
    m_targetTemperature = 0.0;
    m_powerState = false;
    m_thermostatMode = Mode::OFF;
    m_currentTemperature = Temperature_t{0,0,TemperatureTrend::STABLE,0};
    m_wifiStatus = WiFiStatus::DISCONNECTED;
}
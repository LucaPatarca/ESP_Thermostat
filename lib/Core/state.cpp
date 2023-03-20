#include <state.h>
#include <sdebug.h>
#include <ESP_EEPROM.h>

#if LOG_LEVEL > LOG_LEVEL_OFF
const char *thermostatModeNames[] = {"OFF", "ON", "PROGRAM"};
const char *tempTrendNames[] = {"STABLE", "DROP", "RISE"};
const char *wifiStatusNames[] = {"DISCONNECTED", "CONNECTED", "CONNECTING", "CONNECTED_NO_INTERNET"};
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
    INFO("EEPROM.begin(%d)", sizeof(Config_t));
    EEPROM.begin(sizeof(Config_t));
    loadConfig();
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

void State::setWifiStatus(WiFiStatus status)
{
    if(status == m_wifiStatus) return;
    INFO("wifi status is now %s", wifiStatusNames[status]);
    m_wifiStatus = status;
    m_listener->wifiStatusChanged();
}
    
bool State::setProgram(const WeekProgram_t& program)
{
    m_config.program = program;
    return saveConfig();
}

void State::setWifiCredentials(const char *SSID, const char *pass)
{
    size_t ssid_len = strlen(SSID);
    size_t pass_len = strlen(pass);
    if(ssid_len > 64 || pass_len > 64) return;
    strcpy(m_config.wifiSSID, SSID);
    strcpy(m_config.wifiPASS, pass);
    m_config.wifiSet = true;
    saveConfig();
}

void State::setApiCredentials(const char *apiKey, const char *apiSecret, const char *apiDeviceID)
{
    size_t key_len = strlen(apiKey);
    size_t secret_len = strlen(apiSecret);
    size_t id_len = strlen(apiDeviceID);
    if(key_len > 64 || secret_len > 128 || id_len > 32) return;
    strcpy(m_config.apiKey, apiKey);
    strcpy(m_config.apiSecret, apiSecret);
    strcpy(m_config.apiDeviceID, apiDeviceID);
    m_config.apiSet = true;
    saveConfig();
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

Config_t& State::getConfig()
{
    return m_config;
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

    m_config.apiSet = false;
    m_config.wifiSet = false;
    memset(m_config.wifiSSID, 0, 64);
    memset(m_config.wifiPASS, 0, 64);
    memset(m_config.apiKey, 0, 64);
    memset(m_config.apiSecret, 0, 128);
    memset(m_config.apiDeviceID, 0, 32);
    memset(&m_config.program, 0, sizeof(WeekProgram_t));
}

void State::factoryReset()
{
    reset();
    saveConfig();
    ESP.reset();
}

void State::loadConfig()
{
    m_config = EEPROM.get(0, m_config);
}

bool State::saveConfig()
{
    m_config = EEPROM.put(0, m_config);
    bool result = EEPROM.commit();
    if(!result)
    {
        ERROR("cannot save config");
    }
    return result;
}

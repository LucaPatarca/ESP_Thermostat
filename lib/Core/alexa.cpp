#include <alexa.h>
#include <credentials.h>
#include <sdebug.h>

// #define ALEXA_DEBUG

#ifdef ALEXA_DEBUG
#define DEBUG_ESP_PORT Serial
#define NODEBUG_WEBSOCKETS
#define NDEBUG
#endif

#define TEMP_UPDATE_THRESHOLD 0.2f
#define HUMIDITY_UPDATE_THRESHOLD 3

AlexaController* AlexaController::m_instance = nullptr;

AlexaController::AlexaController()
    : m_state(State::Instance())
{
    if(m_instance) { ERROR("Alexa controller already exists"); }
    m_instance = this;
}

void AlexaController::connect()
{
    INFO("alexa connecting...");
    SinricProThermostat &local = SinricPro[THERMOSTAT_ID];
    local.onPowerState([this](const String &device, bool &mode)
                       { return this->onPowerState(device, mode); });
    local.onTargetTemperature([this](const String &device, float &temp)
                              { return this->onTargetTemperature(device, temp); });
    local.onAdjustTargetTemperature([this](const String &device, float &temp)
                                    { return this->onAdjustTargetTemperature(device, temp); });
    local.onThermostatMode([this](const String &device, String &mode)
                           { return this->onThermostatMode(device, mode); });

    local.onSetSetting([this](const String &deviceId, const String &settingId, String &settingValue)
                       { return this->onSetSetting(deviceId, settingId, settingValue); });

    SinricPro.onConnected([]()
                          { INFO("alexa connected to SinricPro."); });
    SinricPro.onDisconnected([]()
                             { INFO("alexa disconnected from SinricPro."); });

    SinricPro.restoreDeviceStates(true);
    SinricPro.begin(APP_KEY, APP_SECRET);

    m_device = &local;
}

bool AlexaController::onPowerState(const String &deviceId, bool &state)
{
    INFO("Thermostat %s turned %s", deviceId.c_str(), state ? "on" : "off");
    m_state.setPowerState(Cause::ALEXA, state);
    return true;
}

bool AlexaController::onAdjustTargetTemperature(const String &deviceId, float &temperatureDelta)
{
    m_state.setTargetTemperature(Cause::ALEXA, m_state.getTargetTemperature() + temperatureDelta); // calculate absolut temperature
    INFO("Thermostat %s changed temperature about %f to %f", deviceId.c_str(), temperatureDelta, m_state.getTargetTemperature());
    temperatureDelta = m_state.getTargetTemperature(); // return absolut temperature
    return true;
}

bool AlexaController::onTargetTemperature(const String &deviceId, float &newTemperature)
{
    INFO("Thermostat %s set temperature to %f", deviceId.c_str(), newTemperature);
    m_state.setTargetTemperature(Cause::ALEXA, newTemperature);
    return true;
}

bool AlexaController::onThermostatMode(const String &deviceId, String &mode)
{
    INFO("Thermostat %s set mode to %s", deviceId.c_str(), mode.c_str());
    if (mode == "AUTO")
        m_state.setThermostatMode(Cause::ALEXA, Mode::PROGRAM);
    else if (mode == "HEAT")
    {
        m_state.setThermostatMode(Cause::ALEXA, Mode::ON);
        m_device->sendPowerStateEvent(true);
    }
    else
    {
        m_state.setThermostatMode(Cause::ALEXA, Mode::OFF);
        m_device->sendPowerStateEvent(false);
    }
    return true;
}

bool AlexaController::onSetSetting(const String &deviceId, const String &settingId, String &settingValue)
{
    INFO("ricevuta impostazione '%s' con valore '%s'", settingId.c_str(), settingValue.c_str());
    if(m_listener)
        m_listener->onSetSetting(settingId, settingValue);
    return true;
}

void AlexaController::handle()
{
    if (m_state.getWifiStatus() == WiFiStatus::CONNECTED)
    {
        SinricPro.handle();
    }
}

void AlexaController::targetTemperatureChanged(Cause cause)
{
    if (cause == Cause::ALEXA)
        return;
    if (m_state.getWifiStatus() == WiFiStatus::CONNECTED)
    {
        INFO("sending target temperature to device");
        m_device->sendTargetTemperatureEvent(m_state.getTargetTemperature());
    }
}

void AlexaController::powerStateChanged(Cause cause)
{
    if (cause == Cause::ALEXA)
        return;
    if (m_state.getWifiStatus() == WiFiStatus::CONNECTED)
    {
        INFO("sending power state to device");
        m_device->sendPowerStateEvent(m_state.getPowerState());
    }
}

void AlexaController::onUpdateEvent(const UpdateEvent_t& event)
{
    if (event.type == UpdateEventType::START)
    {
        if(SinricPro.isConnected())
            SinricPro.stop();
    }
}

void AlexaController::currentTemperatureChanged()
{
    if (m_state.getWifiStatus() == WiFiStatus::CONNECTED)
    {
        auto temp = m_state.getCurrentTemperature();
        if (temp.temp - m_lastSentTemp > TEMP_UPDATE_THRESHOLD ||
            m_lastSentTemp - temp.temp > TEMP_UPDATE_THRESHOLD ||
            temp.humidity - m_lastSentHumidity > HUMIDITY_UPDATE_THRESHOLD ||
            m_lastSentHumidity - temp.humidity > HUMIDITY_UPDATE_THRESHOLD)
        {
            INFO("sending current temperature to device");
            m_device->sendTemperatureEvent(temp.temp, temp.humidity);
            m_lastSentTemp = temp.temp;
            m_lastSentHumidity = temp.humidity;
        }
    }
}

void AlexaController::thermostatModeChanged(Cause cause)
{
    if (cause == Cause::ALEXA)
        return;
    if (m_state.getWifiStatus() == WiFiStatus::CONNECTED)
    {
        INFO("sending thermostat mode to device");
        switch (m_state.getThermostatMode())
        {
        case Mode::OFF:
            m_device->sendThermostatModeEvent("OFF");
            break;
        case Mode::ON:
            m_device->sendThermostatModeEvent("HEAT");
            break;
        case Mode::PROGRAM:
            m_device->sendThermostatModeEvent("AUTO");
            break;

        default:
            break;
        }
    }
}

void AlexaController::wifiStatusChanged()
{
    if (m_state.getWifiStatus() == WiFiStatus::CONNECTED)
    {
        connect();
    }
    else if (m_state.getWifiStatus() == WiFiStatus::DISCONNECTED)
    {
        SinricPro.stop();
    }
}

void AlexaController::addSettingListener(SettingsListener* listener)
{
    m_listener = listener;
}
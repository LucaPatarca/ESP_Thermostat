#include <alexa.h>
#include <credentials.h>

AlexaController::AlexaController()
{
    _wifiConnected = false;
}

void AlexaController::connect()
{
#ifdef ALEXA_DEBUG
    Serial.printf("[AlexaController] connecting...\n");
#endif
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
                          { Serial.printf("Connected to SinricPro\r\n"); });
    SinricPro.onDisconnected([]()
                             { Serial.printf("Disconnected from SinricPro\r\n"); });
    SinricPro.restoreDeviceStates(true);
    SinricPro.begin(APP_KEY, APP_SECRET);

    m_device = &local;
#ifdef ALEXA_DEBUG
    Serial.printf("[AlexaController] connected\n");
#endif
}

bool AlexaController::onPowerState(const String &deviceId, bool &state)
{
    Serial.printf("Thermostat %s turned %s\r\n", deviceId.c_str(), state ? "on" : "off");
    for (StateListener *listener : _listeners)
    {
        listener->onPowerState(state);
        listener->onThermostatMode(state ? Mode::ON : Mode::OFF);
    }
    return true;
}

bool AlexaController::onAdjustTargetTemperature(const String &deviceId, float &temperatureDelta)
{
    _targetTemperature += temperatureDelta; // calculate absolut temperature
    Serial.printf("Thermostat %s changed temperature about %f to %f", deviceId.c_str(), temperatureDelta, _targetTemperature);
    temperatureDelta = _targetTemperature; // return absolut temperature
    for (StateListener *listener : _listeners)
    {
        listener->onTargetTemperature(_targetTemperature);
    }
    return true;
}

bool AlexaController::onTargetTemperature(const String &deviceId, float &newTemperature)
{
    Serial.printf("Thermostat %s set temperature to %f\r\n", deviceId.c_str(), newTemperature);
    _targetTemperature = newTemperature;
    for (StateListener *listener : _listeners)
    {
        listener->onTargetTemperature(_targetTemperature);
    }
    return true;
}

bool AlexaController::onThermostatMode(const String &deviceId, String &mode)
{
    Serial.printf("Thermostat %s set mode to %s\r\n", deviceId.c_str(), mode.c_str());
    if (mode == "AUTO")
        for (StateListener *listener : _listeners)
            listener->onThermostatMode(Mode::PROGRAM);
    else if (mode == "HEAT")
    {
        for (StateListener *listener : _listeners)
        {
            listener->onThermostatMode(Mode::ON);
            listener->onPowerState(true);
        }
        m_device->sendPowerStateEvent(true);
    }
    else
    {
        for (StateListener *listener : _listeners)
        {
            listener->onThermostatMode(Mode::OFF);
            listener->onPowerState(false);
        }
        m_device->sendPowerStateEvent(false);
    }
    return true;
}

bool AlexaController::onSetSetting(const String &deviceId, const String &settingId, String &settingValue)
{
    Serial.printf("ricevuta impostazione '%s' con valore '%s'\n", settingId.c_str(), settingValue.c_str());
    for (StateListener *listener : _listeners)
    {
        listener->onSetSetting(settingId.c_str(), settingValue.c_str());
    }
    return true;
}

void AlexaController::handle()
{
    if (_wifiConnected)
    {
        SinricPro.handle();
    }
}

void AlexaController::onTargetTemperature(float temp)
{
    _targetTemperature = temp;
    if (_wifiConnected)
    {
        m_device->sendTargetTemperatureEvent(temp);
    }
}

void AlexaController::onPowerState(bool state)
{
    if (_wifiConnected)
    {
        m_device->sendPowerStateEvent(state);
    }
}

void AlexaController::onUpdateEvent(UpdateEvent event)
{
    if (event.type == UpdateEventType::START)
    {
        SinricPro.stop();
    }
}

void AlexaController::onCurrentTemperature(Temperature_t temp)
{
    if (_wifiConnected)
    {
        if (temp.temp - _lastSentTemp > TEMP_UPDATE_THRESHOLD || _lastSentTemp - temp.temp > TEMP_UPDATE_THRESHOLD)
        {
            m_device->sendTemperatureEvent(temp.temp, _lastSentHumidity);
            _lastSentTemp = temp.temp;
        }
        else if (temp.humidity - _lastSentHumidity > HUMIDITY_UPDATE_THRESHOLD || _lastSentHumidity - temp.humidity > HUMIDITY_UPDATE_THRESHOLD)
        {
            m_device->sendTemperatureEvent(_lastSentTemp, temp.humidity);
            _lastSentHumidity = temp.humidity;
        }
    }
}

void AlexaController::onThermostatMode(Mode mode)
{
    if (_wifiConnected)
    {
        switch (mode)
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

void AlexaController::onWiFiStatus(WiFiStatus status)
{
    if (status == WiFiStatus::CONNECTED)
    {
        _wifiConnected = true;
        connect();
    }
    else if (status == WiFiStatus::DISCONNECTED)
    {
        SinricPro.stop();
        _wifiConnected = false;
    }
}

void AlexaController::onSetSetting(String key, String value)
{
    // nop
}
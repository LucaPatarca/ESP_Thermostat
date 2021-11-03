#include <alexa.h>
#include <credentials.h>

bool AlexaController::onPowerState(const String &deviceId, bool &state)
{
    Serial.printf("Thermostat %s turned %s\r\n", deviceId.c_str(), state ? "on" : "off");
    m_powerState = state;
    return true;
}

bool AlexaController::onAdjustTargetTemperature(const String &deviceId, float &temperatureDelta)
{
    m_targetTemperature += temperatureDelta; // calculate absolut temperature
    Serial.printf("Thermostat %s changed temperature about %f to %f", deviceId.c_str(), temperatureDelta, m_targetTemperature);
    temperatureDelta = m_targetTemperature; // return absolut temperature
    return true;
}

bool AlexaController::onTargetTemperature(const String &deviceId, float &newTemperature)
{
    Serial.printf("Thermostat %s set temperature to %f\r\n", deviceId.c_str(), newTemperature);
    m_targetTemperature = newTemperature;
    return true;
}

bool AlexaController::onThermostatMode(const String &deviceId, String &mode)
{
    Serial.printf("Thermostat %s set to mode %s\r\n", deviceId.c_str(), mode.c_str());
    return true;
}

bool AlexaController::onSetSetting(const String &deviceId, const String &settingId, String &settingValue)
{
    Serial.printf("ricevuta impostazione %s con valore %s\n", settingId.c_str(), settingValue.c_str());
    return true;
}

void AlexaController::setup()
{
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
}

void AlexaController::handle()
{
    SinricPro.handle();
}

bool AlexaController::isConnected()
{
    return SinricPro.isConnected();
}

void AlexaController::updateCurrentTemperature(float temp, float humidity)
{
    if (
        temp - m_currentTemperature > TEMP_UPDATE_THRESHOLD 
        || m_currentTemperature - temp > TEMP_UPDATE_THRESHOLD 
        || humidity - m_currentHumidity > HUMIDITY_UPDATE_THRESHOLD 
        || m_currentHumidity - humidity > HUMIDITY_UPDATE_THRESHOLD
        )
    {
        m_device->sendTemperatureEvent(temp, humidity);
        m_currentTemperature = temp;
        m_currentHumidity = humidity;
    }
}

bool AlexaController::isOn()
{
    return m_powerState;
}

void AlexaController::setPowerState(bool state)
{
    if (state != m_powerState)
    {
        m_powerState = state;
        m_device->sendPowerStateEvent(m_powerState);
    }
}

float AlexaController::getTargetTemperature()
{
    return m_targetTemperature;
}

void AlexaController::stop()
{
    SinricPro.stop();
}
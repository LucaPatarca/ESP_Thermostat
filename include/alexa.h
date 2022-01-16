#pragma once

#include <SinricPro.h>
#include <SinricProThermostat.h>
#include <state.h>
#include <listeners.h>

class AlexaController
{
public:
    AlexaController(const AlexaController&) = delete;

    static AlexaController& Instance(){
        static AlexaController alexa;
        return alexa;
    }

    void targetTemperatureChanged(Cause);
    void powerStateChanged(Cause);
    void currentTemperatureChanged();
    void thermostatModeChanged(Cause);
    void wifiStatusChanged();

    void onUpdateEvent(const UpdateEvent_t&);

    void addSettingListener(SettingsListener*);

    void connect();
    void handle();
private:
    SinricProThermostat *m_device;
    float m_lastSentTemp;
    float m_lastSentHumidity;
    State& m_state;
    SettingsListener *m_listener;

    bool onTargetTemperature(const String &, float &);
    bool onAdjustTargetTemperature(const String &, float &);
    bool onThermostatMode(const String &, String &);
    bool onPowerState(const String &, bool &);
    bool onSetSetting(const String &, const String &, String &);

    AlexaController();
};
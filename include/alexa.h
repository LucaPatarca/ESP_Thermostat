#pragma once

#include <SinricPro.h>
#include <SinricProThermostat.h>
#include <EventEmitter.h>
#include <StateListener.h>
#include <TemperatureListener.h>
#include <UpdateListener.h>
#include <WiFiListener.h>

// #define ALEXA_DEBUG

#ifdef ALEXA_DEBUG
#define DEBUG_ESP_PORT Serial
#define NODEBUG_WEBSOCKETS
#define NDEBUG
#endif

#define TEMP_UPDATE_THRESHOLD 0.2f
#define HUMIDITY_UPDATE_THRESHOLD 5

class AlexaController : public EventEmitter<StateListener>, public TemperatureListener, public UpdateListener, public StateListener, public WiFiListener
{
private:
    SinricProThermostat *m_device;
    float _targetTemperature;
    float _lastSentTemp;
    float _lastSentHumidity;
    bool _wifiConnected;

    bool onTargetTemperature(const String &, float &);
    bool onAdjustTargetTemperature(const String &, float &);
    bool onThermostatMode(const String &, String &);
    bool onPowerState(const String &, bool &);
    bool onSetSetting(const String &, const String &, String &);

public:
    AlexaController();

    void onTargetTemperature(float) override;
    void onPowerState(bool) override;
    void onUpdateEvent(UpdateEvent) override;
    void onCurrentTemperature(Temperature_t) override;
    void onThermostatMode(Mode) override;
    void onSetSetting(String, String) override;
    void onWiFiStatus(WiFiStatus status) override;

    void connect();
    void handle();
};
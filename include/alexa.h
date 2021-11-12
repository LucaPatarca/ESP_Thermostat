#pragma once

#include <SinricPro.h>
#include <SinricProThermostat.h>
#include <EventEmitter.h>
#include <StateListener.h>
#include <TemperatureListener.h>
#include <UpdateListener.h>

#ifdef ENABLE_DEBUG
#define DEBUG_ESP_PORT Serial
#define NODEBUG_WEBSOCKETS
#define NDEBUG
#define ALEXA_DEBUG
#endif

#define TEMP_UPDATE_THRESHOLD 0.2f
#define HUMIDITY_UPDATE_THRESHOLD 5

class AlexaController : public EventEmitter<StateListener>, public TemperatureListener, public UpdateListener, public StateListener
{
private:
    SinricProThermostat *m_device;
    float _targetTemperature;
    float _lastSentTemp;
    float _lastSentHumidity;

    bool onTargetTemperature(const String &, float &);
    bool onAdjustTargetTemperature(const String &, float &);
    bool onThermostatMode(const String &, String &);
    bool onPowerState(const String &, bool &);
    bool onSetSetting(const String &, const String &, String &);

public:
    void onTargetTemperature(float) override;
    void onPowerState(bool) override;
    void onUpdateEvent(UpdateEvent) override;
    void onCurrentTemperature(Temperature_t) override;

    void connect();
    void handle();
};
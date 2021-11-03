#pragma once

#include <SinricPro.h>
#include <SinricProThermostat.h>
#include <hwio.h>

#ifdef ENABLE_DEBUG
#define DEBUG_ESP_PORT Serial
#define NODEBUG_WEBSOCKETS
#define NDEBUG
#define ALEXA_DEBUG
#endif

#define TEMP_UPDATE_THRESHOLD 0.1f
#define HUMIDITY_UPDATE_THRESHOLD 5

class AlexaController{
    private:
        SinricProThermostat *m_device;
        bool m_powerState;
        float m_targetTemperature;
        float m_currentTemperature;
        float m_currentHumidity;
        HWIOController *m_hwio;

        bool onTargetTemperature(const String&, float&);
        bool onAdjustTargetTemperature(const String&, float&);
        bool onThermostatMode(const String&, String&);
        bool onPowerState(const String&, bool&);
        bool onSetSetting(const String&, const String&, String&);
    public:
        bool isOn();
        void updateCurrentTemperature(float temp, float humidity);
        void setPowerState(bool powerState);
        void handle();
        void setup();
        void stop();
        bool isConnected();
        float getTargetTemperature();

        AlexaController(HWIOController*);
};
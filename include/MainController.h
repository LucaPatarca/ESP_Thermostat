#pragma once

#include <temperature.h>
#include <hwio.h>
#include <wifi.h>
#include <alexa.h>
#include <ota.h>

#ifdef ENABLE_DEBUG
#define MAIN_DEBUG
#endif

#define TEMP_RANGE_LOW 1
#define TEMP_RANGE_HIGH 0.3
#define UPDATE_INTERVAL 30000   //in millisecondi
#define SAFE_TEMP 15

enum TCase
{
    OFF_ABOVE_SAFE_RANGE,
    OFF_BELOW_SAFE_RANGE,
    OFF_IN_SAFE_RANGE,
    BELOW_LOW_RANGE,
    BELOW_RANGE_HIGH_RISING,
    BELOW_RANGE_HIGH_NOT_RISING,
    BELOW_TARGET_DROPPING,
    BELOW_TARGET_NOT_DROPPING,
    ABOVE_TARGET
};

class MainController{
    private:
        bool m_boilerState;
        TemperatureController m_temperature;
        AlexaController *m_alexa;
        HWIOController m_hwio;
        OTAController *m_ota;
        WifiController m_wifi;

        unsigned long m_updateTime;
        void setBoilerState(bool state);
        TCase getTempCase(Temperature temp);
    public:
        MainController();

        void handle();
};
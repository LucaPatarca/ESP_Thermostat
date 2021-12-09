#pragma once

#include <Adafruit_SSD1306.h>
#include <temperature.h>
#include <BoilerListener.h>
#include <StateListener.h>
#include <TemperatureListener.h>
#include <UpdateListener.h>
#include <WiFiListener.h>
#include <screen/ScreenElement.h>

// #define HWIO_DEBUG 

class HWIOController : public BoilerListener, public StateListener, public TemperatureListener, public UpdateListener, public WiFiListener
{
private:
    Adafruit_SSD1306 *_display;
    float _lastTargetTemp;
    float _lastPowerState;
    Mode _lastMode;

    ScreenElement<WiFiStatus> *_wifiIcon;
    ScreenElement<float> *_targetTemp;
    ScreenElement<float> *_currentTemp;
    ScreenElement<float> *_currentHumidity;

    void setDisplay(int, int, int);

public:
    HWIOController();

    void onBoilerState(bool) override;

    void onPowerState(bool) override;
    void onTargetTemperature(float) override;
    void onThermostatMode(Mode) override;
    void onSetSetting(String, String) override;
    void onCurrentTemperature(Temperature_t) override;
    void onUpdateEvent(UpdateEvent) override;
    void onWiFiStatus(WiFiStatus) override;

    void handle();
    void init();
};
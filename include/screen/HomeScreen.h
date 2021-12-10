#pragma once

#include <screen/Screen.h>
#include <BoilerListener.h>
#include <StateListener.h>
#include <TemperatureListener.h>
#include <WiFiListener.h>
#include <screen/ScreenElement.h>

//TODO continuare

class HomeScreen: public Screen, public BoilerListener, public StateListener, public TemperatureListener, public WiFiListener {
private:
    float _lastTargetTemp;
    float _lastPowerState;

    ScreenElement<WiFiStatus> *_wifiIcon;
    ScreenElement<float> *_targetTemp;
    ScreenElement<float> *_currentTemp;
    ScreenElement<float> *_currentHumidity;
    ScreenElement<Mode> *_thermoMode;
    ScreenElement<bool> *_boilerState;
    ScreenElement<TemperatureTrend> *_tempTrend;
public:
    HomeScreen(Adafruit_SSD1306 *display);

    void onBoilerState(bool) override;
    void onPowerState(bool) override;
    void onTargetTemperature(float) override;
    void onThermostatMode(Mode) override;
    void onSetSetting(String, String) override;
    void onCurrentTemperature(Temperature_t) override;
    void onWiFiStatus(WiFiStatus) override;

    void draw() override;
    void refresh() override;
};
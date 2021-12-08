#pragma once

#include <StateListener.h>
#include <TemperatureListener.h>
#include <BoilerListener.h>
#include <ESP8266HTTPClient.h>

#define LOGGER_DEBUG

class Logger : public StateListener, public TemperatureListener, public BoilerListener
{
private:
    WiFiClient _client;
    HTTPClient _http;
    Temperature _temp;
    float _target;
    bool _power;
    bool _boiler;

    void _sendLog();
public:
    void onBoilerState(bool) override;
    void onCurrentTemperature(Temperature) override;
    void onThermostatMode(Mode) override;
    void onPowerState(bool) override;
    void onTargetTemperature(float) override;
    void onSetSetting(String, String) override;
};
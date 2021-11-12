#pragma once

#include <StateListener.h>
#include <TemperatureListener.h>
#include <BoilerListener.h>
#include <ESP8266HTTPClient.h>


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
    void onBoilerState(bool);
    void onCurrentTemperature(Temperature);
    void onPowerState(bool);
    void onTargetTemperature(float);
};
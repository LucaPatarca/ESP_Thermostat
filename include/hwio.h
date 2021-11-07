#pragma once

#include <Adafruit_SSD1306.h>
#include <temperature.h>
#include <BoilerListener.h>
#include <StateListener.h>
#include <TemperatureListener.h>
#include <UpdateListener.h>
#include <WiFiListener.h>

#define HWIO_DEBUG

#ifdef ENABLE_DEBUG
#define HWIO_DEBUG
#endif

class HWIOController : public BoilerListener, public StateListener, public TemperatureListener, public UpdateListener, public WiFiListener
{
private:
    Adafruit_SSD1306 *_display;
    float _lastTargetTemp;
    float _lastPowerState;

    void setDisplay(int, int, int);

public:
    HWIOController();

    void onBoilerState(bool) override;

    void onPowerState(bool) override;
    void onTargetTemperature(float) override;

    void onCurrentTemperature(Temperature_t) override;

    void onUpdateEvent(UpdateEvent);

    void onWiFiStatus(WiFiStatus);

    void init();
};
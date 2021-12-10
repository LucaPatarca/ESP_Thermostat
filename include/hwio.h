#pragma once

#include <Adafruit_SSD1306.h>
#include <temperature.h>
#include <BoilerListener.h>
#include <StateListener.h>
#include <TemperatureListener.h>
#include <UpdateListener.h>
#include <WiFiListener.h>
#include <screen/HomeScreen.h>
#include <screen/UpdateScreen.h>

// #define HWIO_DEBUG 

class HWIOController : public BoilerListener, public StateListener, public TemperatureListener, public UpdateListener, public WiFiListener
{
private:
    Adafruit_SSD1306 *_display;
    
    HomeScreen *_homeScreen;
    UpdateScreen *_updateScreen;
    
    Screen *_activeScreen;

    void _setActiveScreen(Screen *screen);
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
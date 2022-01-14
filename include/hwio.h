#pragma once

#include <Adafruit_SSD1306.h>
#include <temperature.h>
#include <screen/HomeScreen.h>
#include <screen/UpdateScreen.h>
#include <screen/TimeScreen.h>
#include <listeners.h>

// #define HWIO_DEBUG 
#define SCREEN_INTERVAL 10000    //in milliseconds

class HWIOController
{
public:
    HWIOController(HWIOController&) = delete;

    static HWIOController& Instance(){
        static HWIOController controller;
        return controller;
    }
    
    void boilerStateChanged();
    void powerStateChanged();
    void targetTemperatureChanged();
    void thermostatModeChanged();
    void currentTemperatureChanged();
    void wifiStatusChanged();

    void onUpdateEvent(UpdateEvent_t&);

    void handle();
    void init();
private:
    Adafruit_SSD1306 *m_display;
    unsigned long m_lastChange;
    
    HomeScreen *m_homeScreen;
    UpdateScreen *m_updateScreen;
    TimeScreen *m_timeScreen;
    
    Screen *m_activeScreen;

    void _setActiveScreen(Screen *screen);

    HWIOController();
};
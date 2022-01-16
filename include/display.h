#pragma once

#include <Adafruit_SSD1306.h>
#include <temperature.h>
#include <screen/HomeScreen.h>
#include <screen/UpdateScreen.h>
#include <screen/TimeScreen.h>
#include <screen/NotificationScreen.h>
#include <listeners.h>

// #define HWIO_DEBUG 

//TODO optimize object parameters by moving instead of copying (when possible)

class DisplayController
{
public:
    DisplayController(const DisplayController&) = delete;

    static DisplayController& Instance(){
        static DisplayController controller;
        return controller;
    }
    
    void boilerStateChanged();
    void powerStateChanged();
    void targetTemperatureChanged(Cause);
    void thermostatModeChanged(Cause);
    void currentTemperatureChanged();
    void wifiStatusChanged();

    void onUpdateEvent(const UpdateEvent_t&);

    void handle();
private:
    Adafruit_SSD1306 m_display;
    unsigned long m_lastChange;
    int m_changeInterval;
    
    HomeScreen m_homeScreen;
    UpdateScreen m_updateScreen;
    TimeScreen m_timeScreen;
    NotificationScreen m_notificationScreen;
    
    Screen *m_activeScreen;

    void _setActiveScreen(Screen *);
    void _notify(const String&);

    DisplayController();
};
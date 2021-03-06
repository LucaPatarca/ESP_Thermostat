#pragma once

#ifndef NO_DISPLAY

#include <Adafruit_SSD1306.h>
#include <temperature.h>
#include <screen/HomeScreen.h>
#include <screen/UpdateScreen.h>
#include <screen/TimeScreen.h>
#include <screen/NotificationScreen.h>
#include <listeners.h>

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

    void notify(const char*, uint32_t);
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

    DisplayController();
};

#define IF_DISPLAY_ENABLED(...) __VA_ARGS__
#else
#define IF_DISPLAY_ENABLED(...)
#endif
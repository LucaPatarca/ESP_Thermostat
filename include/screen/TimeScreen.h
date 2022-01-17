#pragma once

#ifndef NO_DISPLAY

#include <screen/components.h>

class TimeScreen: public Screen
{
private:
    Clock m_clock;
    WifiIcon m_wifiIcon;
    ThermoMode m_thermoMode;
public:
    TimeScreen(Adafruit_SSD1306 &display);

    void wifiStatusChanged();
    void thermostatModeChanged();

    void draw() override;
    void refresh() override;
};

#endif

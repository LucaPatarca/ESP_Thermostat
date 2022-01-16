#pragma once

#include <screen/components.h>
#include <state.h>

class HomeScreen: public Screen {
private:
    TargetTemp m_targetTemp;
    CurrentTemp m_currentTemp;
    CurrentHumidity m_currentHumidity;
    BoilerState m_boilerState;
    TempTrend m_tempTrend;
public:
    HomeScreen(Adafruit_SSD1306 &display);

    void boilerStateChanged();
    void powerStateChanged();
    void targetTemperatureChanged();
    void currentTemperatureChanged();

    void draw() override;
    void refresh() override;
};
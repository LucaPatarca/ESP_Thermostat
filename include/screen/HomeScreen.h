#pragma once

#include <screen/components.h>
#include <state.h>

//TODO continuare

class HomeScreen: public Screen {
private:
    float _lastTargetTemp;
    float _lastPowerState;

    ScreenElement *_targetTemp;
    ScreenElement *_currentTemp;
    ScreenElement *_currentHumidity;
    ScreenElement *_boilerState;
    ScreenElement *_tempTrend;
public:
    HomeScreen(Adafruit_SSD1306 *display);

    void boilerStateChanged();
    void powerStateChanged();
    void targetTemperatureChanged();
    void currentTemperatureChanged();

    void draw() override;
    void refresh() override;
};
#pragma once

#include <Adafruit_SSD1306.h>

class Screen{
protected:
    Adafruit_SSD1306 *_display;
public:
    Screen(Adafruit_SSD1306 *display);
    virtual void draw() = 0;
    virtual void refresh() = 0;
};
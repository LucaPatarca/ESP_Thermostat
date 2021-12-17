#pragma once

#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

template<typename Status>
class ScreenElement{
protected:
    Adafruit_SSD1306 *_display;
    Status _status;
    bool _needUpdate;
    int _x;
    int _y;

    void _setDisplay(int size, int x, int y);
public:
    ScreenElement(Adafruit_SSD1306 *_display, int x, int y);

    void setStatus(Status status);
    virtual void draw();
    void refresh();

    virtual void drawImpl() = 0;
};
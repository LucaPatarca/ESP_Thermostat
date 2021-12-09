#pragma once

#include <Adafruit_SSD1306.h>

template<typename Status>
class ScreenElement{
protected:
    Adafruit_SSD1306 *_display;
    Status _status;
    bool _needUpdate;

    void _setDisplay(int size, int x, int y);
public:
    ScreenElement(Adafruit_SSD1306 *_display);

    void setStatus(Status status);
    void draw();

    virtual void drawImpl() = 0;
};
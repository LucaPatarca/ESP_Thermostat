#pragma once

#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

class Screen{
protected:
    Adafruit_SSD1306 *_display;
public:
    Screen(Adafruit_SSD1306 *display);
    virtual void draw() = 0;
    virtual void refresh() = 0;
};

class ScreenElement{
protected:
    Adafruit_SSD1306 *_display;
    bool _needUpdate;
    int _x;
    int _y;

    void _setDisplay(int size, int x, int y);
public:
    ScreenElement(Adafruit_SSD1306 *_display, int x, int y);

    virtual void draw();
    void refresh();

    virtual void drawImpl() = 0;
};

class AnimatedScreenElement: public ScreenElement
{
protected:
    int _animStatus;
    int _interval;
    unsigned long _lastTick;
public:
    AnimatedScreenElement(Adafruit_SSD1306 *display, int interval, int x, int y);

    void tick();
    void draw() override;
    virtual void tickImpl() = 0;
};
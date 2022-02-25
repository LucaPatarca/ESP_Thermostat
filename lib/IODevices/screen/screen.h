#pragma once

#ifndef NO_DISPLAY

#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>

class Screen{
protected:
    Adafruit_SSD1306 &m_display;
public:
    Screen(Adafruit_SSD1306 &display);
    virtual void draw() = 0;
    virtual void refresh() = 0;
};

class ScreenElement{
protected:
    Adafruit_SSD1306 &m_display;
    bool m_needUpdate;
    int m_x;
    int m_y;

    void _setDisplay(int size, int x, int y);
public:
    ScreenElement(Adafruit_SSD1306 &m_display, int x, int y);

    virtual void draw();
    void refresh();

    virtual void drawImpl() = 0;
};

class AnimatedScreenElement: public ScreenElement
{
protected:
    int m_animStatus;
    int m_interval;
    unsigned long m_lastTick;
public:
    AnimatedScreenElement(Adafruit_SSD1306 &display, int interval, int x, int y);

    void tick();
    void draw() override;
    virtual void tickImpl() = 0;
};

#endif
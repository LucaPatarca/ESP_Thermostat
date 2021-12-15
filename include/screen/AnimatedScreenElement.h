#pragma once

#include <screen/ScreenElement.h>

template<typename Status>
class AnimatedScreenElement: public ScreenElement<Status>
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
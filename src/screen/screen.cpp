#include <screen/screen.h>

Screen::Screen(Adafruit_SSD1306 *display){
    _display = display;
}

AnimatedScreenElement::AnimatedScreenElement(Adafruit_SSD1306 *display, int interval, int x, int y)
    : ScreenElement(display, x, y)
{
    _interval = interval;
}

void AnimatedScreenElement::tick()
{
    if(millis() > _lastTick + _interval){
        tickImpl();
        this->_needUpdate = true;
        _lastTick = millis();
    }
}

void AnimatedScreenElement::draw()
{
    this->tick();
    ScreenElement::draw();
}

ScreenElement::ScreenElement(Adafruit_SSD1306 *display, int x, int y)
{
    _display = display;
    _x = x;
    _y = y;
    _needUpdate = true;
}

void ScreenElement::_setDisplay(int size, int x, int y){
    _display->setFont();
    _display->setTextSize(size);
    _display->setTextColor(WHITE, BLACK);
    _display->setCursor(x, y);
}

void ScreenElement::draw()
{
    if (_needUpdate)
    {
        drawImpl();

        _needUpdate = false;
    }
}

void ScreenElement::refresh(){
    _needUpdate = true;
}
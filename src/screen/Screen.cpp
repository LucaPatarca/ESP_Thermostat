#include <screen/screen.h>

Screen::Screen(Adafruit_SSD1306 &display)
    : m_display(display)
{
}

AnimatedScreenElement::AnimatedScreenElement(Adafruit_SSD1306 &display, int interval, int x, int y)
    : ScreenElement(display, x, y)
{
    m_interval = interval;
}

void AnimatedScreenElement::tick()
{
    if(millis() > m_lastTick + m_interval){
        tickImpl();
        this->m_needUpdate = true;
        m_lastTick = millis();
    }
}

void AnimatedScreenElement::draw()
{
    this->tick();
    ScreenElement::draw();
}

ScreenElement::ScreenElement(Adafruit_SSD1306 &display, int x, int y)
    : m_display(display),
    m_needUpdate(true),
    m_x(x),
    m_y(y)
{
}

void ScreenElement::_setDisplay(int size, int x, int y){
    m_display.setFont();
    m_display.setTextSize(size);
    m_display.setTextColor(WHITE, BLACK);
    m_display.setCursor(x, y);
}

void ScreenElement::draw()
{
    if (m_needUpdate)
    {
        drawImpl();

        m_needUpdate = false;
    }
}

void ScreenElement::refresh(){
    m_needUpdate = true;
}
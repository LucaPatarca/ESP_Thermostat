#include <screen/ScreenElement.h>

#include <WiFiListener.h>

template<typename Status>
ScreenElement<Status>::ScreenElement(Adafruit_SSD1306 *display)
{
    _display = display;
    _needUpdate = true;
}

template<typename Status>
void ScreenElement<Status>::_setDisplay(int size, int x, int y){
    _display->setTextSize(size);
    _display->setTextColor(WHITE, BLACK);
    _display->setCursor(x, y);
}

template<typename Status>
void ScreenElement<Status>::setStatus(Status status)
{
    if(status != _status){
        _needUpdate = true;
    }
    _status = status;
}

template<typename Status>
void ScreenElement<Status>::draw()
{
    if (_needUpdate)
    {
        drawImpl();
    }
}

template class ScreenElement<WiFiStatus>;
template class ScreenElement<float>;
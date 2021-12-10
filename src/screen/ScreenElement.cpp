#include <screen/ScreenElement.h>

#include <WiFiListener.h>
#include <StateListener.h>
#include <TemperatureListener.h>
#include <UpdateListener.h>
#include <screen/components/Clock.h>

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

        _needUpdate = false;
    }
}

template<typename Status>
void ScreenElement<Status>::refresh(){
    _needUpdate = true;
}

template class ScreenElement<WiFiStatus>;
template class ScreenElement<Mode>;
template class ScreenElement<TemperatureTrend>;
template class ScreenElement<UpdateEvent_t>;
template class ScreenElement<Time_t>;
template class ScreenElement<float>;
template class ScreenElement<bool>;
template class ScreenElement<int>;
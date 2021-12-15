#include <screen/AnimatedScreenElement.h>
#include <WiFiListener.h>
#include <ntime.h>

template<typename Status>
AnimatedScreenElement<Status>::AnimatedScreenElement(Adafruit_SSD1306 *display, int interval, int x, int y): ScreenElement<Status>(display, x, y)
{
    _interval = interval;
}

template<typename Status>
void AnimatedScreenElement<Status>::tick()
{
    if(millis() > _lastTick + _interval){
        tickImpl();
        this->_needUpdate = true;
        _lastTick = millis();
    }
}

template<typename Status>
void AnimatedScreenElement<Status>::draw()
{
    this->tick();
    ScreenElement<Status>::draw();
}

template class AnimatedScreenElement<WiFiStatus>;
template class AnimatedScreenElement<Time_t>;

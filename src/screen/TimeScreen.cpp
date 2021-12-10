#include <screen/TimeScreen.h>
#include <screen/components/WifiIcon.h>

TimeScreen::TimeScreen(Adafruit_SSD1306 *display) : Screen(display)
{
    _clock = new Clock(_display);
    _clock->setStatus(Time->getTime());

    _wifiIcon = new WifiIcon(_display);
}

void TimeScreen::onWiFiStatus(WiFiStatus status){
    _wifiIcon->setStatus(status);
}

void TimeScreen::draw()
{
    _clock->setStatus(Time->getTime());
    _clock->draw();
    _wifiIcon->draw();
}

void TimeScreen::refresh()
{
    _clock->refresh();
    _wifiIcon->refresh();
}

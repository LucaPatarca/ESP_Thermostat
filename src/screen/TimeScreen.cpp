#include <screen/TimeScreen.h>

TimeScreen::TimeScreen(Adafruit_SSD1306 *display) : Screen(display)
{
    _clock = new Clock(_display, 800, 0, 0);

    _wifiIcon = new WifiIcon(_display, 800, 104, 36);
    _thermoMode = new ThermoMode(_display, 104, 0);
}

void TimeScreen::wifiStatusChanged(){
    _wifiIcon->refresh();
}

void TimeScreen::thermostatModeChanged()
{
    _thermoMode->refresh();
}

void TimeScreen::draw()
{
    _clock->draw();
    _wifiIcon->draw();
    _thermoMode->draw();
}

void TimeScreen::refresh()
{
    _clock->refresh();
    _wifiIcon->refresh();
    _thermoMode->refresh();
}

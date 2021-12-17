#include <screen/TimeScreen.h>
#include <screen/components/WifiIcon.h>
#include <screen/components/ThermoMode.h>

TimeScreen::TimeScreen(Adafruit_SSD1306 *display) : Screen(display)
{
    _clock = new Clock(_display, 800, 0, 0);
    _clock->setStatus(Time->getTime());

    _wifiIcon = new WifiIcon(_display, 800, 104, 36);
    _thermoMode = new ThermoMode(_display, 104, 0);
}

void TimeScreen::onWiFiStatus(WiFiStatus status){
    _wifiIcon->setStatus(status);
}

void TimeScreen::onPowerState(bool value)
{
    //nop
}

void TimeScreen::onTargetTemperature(float value)
{
    //nop
}

void TimeScreen::onSetSetting(String, String)
{
    //nop
}

void TimeScreen::onThermostatMode(Mode value)
{
    _thermoMode->setStatus(value);
}


void TimeScreen::draw()
{
    _clock->setStatus(Time->getTime());
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

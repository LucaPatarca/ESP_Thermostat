#include <screen/HomeScreen.h>

#include <screen/components/WifiIcon.h>
#include <screen/components/TargetTemp.h>
#include <screen/components/CurrentTemp.h>
#include <screen/components/CurrentHumidity.h>
#include <screen/components/ThermoMode.h>
#include <screen/components/BoilerState.h>
#include <screen/components/TempTrend.h>

HomeScreen::HomeScreen(Adafruit_SSD1306 *display) : Screen(display)
{
    _wifiIcon = new WifiIcon(_display);
    _targetTemp = new TargetTemp(_display);
    _currentTemp = new CurrentTemp(_display);
    _currentHumidity = new CurrentHumidity(_display);
    _thermoMode = new ThermoMode(_display);
    _boilerState = new BoilerState(_display);
    _tempTrend = new TempTrend(_display);
}

void HomeScreen::onBoilerState(bool state){
    _boilerState->setStatus(state);
}

void HomeScreen::onPowerState(bool state)
{
    _targetTemp->setStatus(state?_lastTargetTemp:0);
    _lastPowerState = state;
}

void HomeScreen::onTargetTemperature(float temp)
{
    if (_lastPowerState)
    {
        _targetTemp->setStatus(temp);
    }
    _lastTargetTemp = temp;
}

void HomeScreen::onThermostatMode(Mode mode)
{
    _thermoMode->setStatus(mode);
}

void HomeScreen::onSetSetting(String, String)
{
    //nop
}

void HomeScreen::onCurrentTemperature(Temperature_t temp)
{
    _currentTemp->setStatus(temp.temp);
    _currentHumidity->setStatus(temp.humidity);
    _tempTrend->setStatus(temp.trend);
}

void HomeScreen::onWiFiStatus(WiFiStatus status)
{
    _wifiIcon->setStatus(status);
}

void HomeScreen::draw()
{
    _wifiIcon->draw();
    _targetTemp->draw();
    _currentTemp->draw();
    _currentHumidity->draw();
    _thermoMode->draw();
    _boilerState->draw();
    _tempTrend->draw();
}

void HomeScreen::refresh(){
    _wifiIcon->refresh();
    _targetTemp->refresh();
    _currentTemp->refresh();
    _currentHumidity->refresh();
    _thermoMode->refresh();
    _boilerState->refresh();
    _tempTrend->refresh();
}
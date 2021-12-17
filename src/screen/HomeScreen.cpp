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
    _targetTemp = new TargetTemp(_display, 0, 25);
    _currentTemp = new CurrentTemp(_display, 0, 0);
    _currentHumidity = new CurrentHumidity(_display, 0, 44);
    _boilerState = new BoilerState(_display, 98, 38);
    _tempTrend = new TempTrend(_display, 104, 0);
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
    //nop
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

void HomeScreen::draw()
{
    _targetTemp->draw();
    _currentTemp->draw();
    _currentHumidity->draw();
    _boilerState->draw();
    _tempTrend->draw();
}

void HomeScreen::refresh(){
    _targetTemp->refresh();
    _currentTemp->refresh();
    _currentHumidity->refresh();
    _boilerState->refresh();
    _tempTrend->refresh();
}
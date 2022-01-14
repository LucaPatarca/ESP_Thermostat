#include <screen/HomeScreen.h>

HomeScreen::HomeScreen(Adafruit_SSD1306 *display) : Screen(display)
{
    _targetTemp = new TargetTemp(_display, 0, 25);
    _currentTemp = new CurrentTemp(_display, 0, 0);
    _currentHumidity = new CurrentHumidity(_display, 0, 44);
    _boilerState = new BoilerState(_display, 98, 38);
    _tempTrend = new TempTrend(_display, 104, 0);
}

void HomeScreen::boilerStateChanged(){
    _boilerState->refresh();
}

void HomeScreen::powerStateChanged()
{
    _targetTemp->refresh();
}

void HomeScreen::targetTemperatureChanged()
{
    if (State::Instance().getPowerState())
    {
        _targetTemp->refresh();
    }
}

void HomeScreen::currentTemperatureChanged()
{
    _currentTemp->refresh();
    _currentHumidity->refresh();
    _tempTrend->refresh();
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
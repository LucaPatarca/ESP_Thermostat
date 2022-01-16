#include <screen/HomeScreen.h>

HomeScreen::HomeScreen(Adafruit_SSD1306 &display) 
    : Screen(display),
    m_targetTemp(TargetTemp(display, 0, 25)),
    m_currentTemp(CurrentTemp(display, 0, 0)),
    m_currentHumidity(CurrentHumidity(display, 0, 44)),
    m_boilerState(BoilerState(display, 98, 38)),
    m_tempTrend(TempTrend(display, 104, 0))
{
}

void HomeScreen::boilerStateChanged(){
    m_boilerState.refresh();
}

void HomeScreen::powerStateChanged()
{
    m_targetTemp.refresh();
}

void HomeScreen::targetTemperatureChanged()
{
    if (State::Instance().getPowerState())
    {
        m_targetTemp.refresh();
    }
}

void HomeScreen::currentTemperatureChanged()
{
    m_currentTemp.refresh();
    m_currentHumidity.refresh();
    m_tempTrend.refresh();
}

void HomeScreen::draw()
{
    m_targetTemp.draw();
    m_currentTemp.draw();
    m_currentHumidity.draw();
    m_boilerState.draw();
    m_tempTrend.draw();
}

void HomeScreen::refresh(){
    m_targetTemp.refresh();
    m_currentTemp.refresh();
    m_currentHumidity.refresh();
    m_boilerState.refresh();
    m_tempTrend.refresh();
}
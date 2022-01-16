#include <screen/TimeScreen.h>

TimeScreen::TimeScreen(Adafruit_SSD1306 &display) 
    : Screen(display),
    m_clock(Clock(display, 800, 0, 0)),
    m_wifiIcon(WifiIcon(display, 800, 104, 36)),
    m_thermoMode(ThermoMode(display, 104, 0))
{
}

void TimeScreen::wifiStatusChanged(){
    m_wifiIcon.refresh();
}

void TimeScreen::thermostatModeChanged()
{
    m_thermoMode.refresh();
}

void TimeScreen::draw()
{
    m_clock.draw();
    m_wifiIcon.draw();
    m_thermoMode.draw();
}

void TimeScreen::refresh()
{
    m_clock.refresh();
    m_wifiIcon.refresh();
    m_thermoMode.refresh();
}

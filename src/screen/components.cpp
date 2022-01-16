#include <screen/components.h>
#include <screen/icons.h>
#include <state.h>

void BoilerState::drawImpl(){
	if(State::Instance().getBoilerState()){
    	m_display.drawBitmap(m_x, m_y, fire, 24, 24, WHITE);
	}else{
		m_display.fillRect(m_x, m_y, 24, 24, BLACK);
	}
}

#define DOTS_VISIBLE 0
#define DOTS_NOT_VISIBLE 1

void Clock::drawImpl(){
    auto _status = State::Instance().getTime();
    m_display.fillRect(m_x, m_y, 90, 50, BLACK);
    _setDisplay(1, m_x, m_y+18);
    m_display.setFont(&FreeSans18pt7b);
    m_display.printf("%02d%s%02d", _status.hour, m_animStatus == DOTS_VISIBLE ? ":" : " ", _status.minutes);
    _setDisplay(1, m_x, m_y + 42);
    m_display.setFont(&FreeSans12pt7b);
    String s[] = {"Lun", "Mar", "Mer", "Gio", "Ven", "Sab", "Dom"};
    m_display.printf("%s", s[_status.day].c_str());
}

void Clock::tickImpl(){
    m_animStatus = (m_animStatus + 1) % 2;
}

void CurrentHumidity::drawImpl(){
    m_display.fillRect(m_x, m_y, 48, 18, BLACK);
    _setDisplay(1, m_x, m_y+12);
    m_display.setFont(&FreeSans12pt7b);
    m_display.printf("%.1f%%", State::Instance().getCurrentTemperature().humidity);
}

void CurrentTemp::drawImpl(){
    m_display.fillRect(m_x, m_y, 92, 24, BLACK);
    _setDisplay(1, m_x, m_y + 18);
    m_display.setFont(&FreeSans18pt7b);
    m_display.printf("%.1f", State::Instance().getCurrentTemperature().temp);
    m_display.drawCircle(m_x+72, m_y+8, 4, WHITE);
    m_display.setFont(&FreeSans12pt7b);
    m_display.setCursor(m_x+80, m_y+14);
    m_display.print("c");
}

void TargetTemp::drawImpl(){
    State& state = State::Instance();
    m_display.fillRect(m_x, m_y, 72, 19, BLACK);
    _setDisplay(1, m_x, m_y+12);
    m_display.setFont(&FreeSans12pt7b);
    if (state.getPowerState())
        m_display.printf("%.1f c", state.getTargetTemperature());
    else
        m_display.printf("--.- c");
}

void TempTrend::drawImpl(){
    m_display.fillRect(m_x, m_y, 24, 24, BLACK);

    switch(State::Instance().getCurrentTemperature().trend)
    {
    case TemperatureTrend::DROP:
        m_display.drawBitmap(m_x, m_y, arrowDown, 24, 24, WHITE);
        break;
    case TemperatureTrend::RISE:
        m_display.drawBitmap(m_x, m_y, arrowUp, 24, 24, WHITE);
        break;

    default:
        break;
    }
}

void ThermoMode::drawImpl(){
    m_display.fillRect(m_x, m_y, 24, 24, BLACK);

    switch(State::Instance().getThermostatMode())
    {
    case Mode::PROGRAM:
        m_display.drawBitmap(m_x, m_y, calendar, 24, 24, WHITE);
        break;
    case Mode::ON:
        m_display.drawBitmap(m_x, m_y, sun, 24, 24, WHITE);
        break;
    case Mode::OFF:
        m_display.drawBitmap(m_x, m_y, moon, 24, 24, WHITE);
        break;
    
    default:
        break;
    }
}

#define BAR1 0
#define BAR2 1
#define BAR3 2

void WifiIcon::drawImpl()
{
    m_display.fillRect(m_x, m_y, 24, 24, BLACK);

    switch (State::Instance().getWifiStatus())
    {
    case WiFiStatus::CONNECTING:
        switch (m_animStatus)
        {
        case BAR1:
            m_display.drawBitmap(m_x, m_y, wifi1bar, 24, 24, WHITE);
            break;
        case BAR2:
            m_display.drawBitmap(m_x, m_y, wifi2bars, 24, 24, WHITE);
            break;

        default:
            break;
        }
        break;
    case WiFiStatus::CONNECTED:
        m_display.drawBitmap(m_x, m_y, wifiConnected, 24, 24, WHITE);
        break;
    case WiFiStatus::DISCONNECTED:
        m_display.drawBitmap(m_x, m_y, wifiDisconnected, 24, 24, WHITE);

    default:
        break;
    }
}

void WifiIcon::tickImpl()
{
    m_animStatus = (m_animStatus + 1) % 2;
}
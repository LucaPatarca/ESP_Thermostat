#include <screen/components.h>
#include <screen/icons.h>
#include <state.h>

void BoilerState::drawImpl(){
	if(State::Instance().getBoilerState()){
    	_display->drawBitmap(_x, _y, fire, 24, 24, WHITE);
	}else{
		_display->fillRect(_x, _y, 24, 24, BLACK);
	}
}

#define DOTS_VISIBLE 0
#define DOTS_NOT_VISIBLE 1

void Clock::drawImpl(){
    auto _status = State::Instance().getTime();
    _display->fillRect(_x, _y, 90, 50, BLACK);
    _setDisplay(1, _x, _y+18);
    _display->setFont(&FreeSans18pt7b);
    _display->printf("%02d%s%02d", _status.hour, _animStatus == DOTS_VISIBLE ? ":" : " ", _status.minutes);
    _setDisplay(1, _x, _y + 42);
    _display->setFont(&FreeSans12pt7b);
    String s[] = {"Lun", "Mar", "Mer", "Gio", "Ven", "Sab", "Dom"};
    _display->printf("%s", s[_status.day].c_str());
}

void Clock::tickImpl(){
    _animStatus = (_animStatus + 1) % 2;
}

void CurrentHumidity::drawImpl(){
    _display->fillRect(_x, _y, 48, 18, BLACK);
    _setDisplay(1, _x, _y+12);
    _display->setFont(&FreeSans12pt7b);
    _display->printf("%.1f%%", State::Instance().getCurrentTemperature().humidity);
}

void CurrentTemp::drawImpl(){
    _display->fillRect(_x, _y, 92, 24, BLACK);
    _setDisplay(1, _x, _y + 18);
    _display->setFont(&FreeSans18pt7b);
    _display->printf("%.1f", State::Instance().getCurrentTemperature().temp);
    _display->drawCircle(_x+72, _y+8, 4, WHITE);
    _display->setFont(&FreeSans12pt7b);
    _display->setCursor(_x+80, _y+14);
    _display->print("c");
}

void TargetTemp::drawImpl(){
    State& state = State::Instance();
    _display->fillRect(_x, _y, 72, 19, BLACK);
    _setDisplay(1, _x, _y+12);
    _display->setFont(&FreeSans12pt7b);
    if (state.getPowerState())
        _display->printf("%.1f c", state.getTargetTemperature());
    else
        _display->printf("--.- c");
}

void TempTrend::drawImpl(){
    _display->fillRect(_x, _y, 24, 24, BLACK);

    switch(State::Instance().getCurrentTemperature().trend)
    {
    case TemperatureTrend::DROP:
        _display->drawBitmap(_x, _y, arrowDown, 24, 24, WHITE);
        break;
    case TemperatureTrend::RISE:
        _display->drawBitmap(_x, _y, arrowUp, 24, 24, WHITE);
        break;

    default:
        break;
    }
}

void ThermoMode::drawImpl(){
    _display->fillRect(_x, _y, 24, 24, BLACK);

    switch(State::Instance().getThermostatMode())
    {
    case Mode::PROGRAM:
        _display->drawBitmap(_x, _y, calendar, 24, 24, WHITE);
        break;
    case Mode::ON:
        _display->drawBitmap(_x, _y, sun, 24, 24, WHITE);
        break;
    case Mode::OFF:
        _display->drawBitmap(_x, _y, moon, 24, 24, WHITE);
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
    _display->fillRect(_x, _y, 24, 24, BLACK);

    switch (State::Instance().getWifiStatus())
    {
    case WiFiStatus::CONNECTING:
        switch (_animStatus)
        {
        case BAR1:
            _display->drawBitmap(_x, _y, wifi1bar, 24, 24, WHITE);
            break;
        case BAR2:
            _display->drawBitmap(_x, _y, wifi2bars, 24, 24, WHITE);
            break;

        default:
            break;
        }
        break;
    case WiFiStatus::CONNECTED:
        _display->drawBitmap(_x, _y, wifiConnected, 24, 24, WHITE);
        break;
    case WiFiStatus::DISCONNECTED:
        _display->drawBitmap(_x, _y, wifiDisconnected, 24, 24, WHITE);

    default:
        break;
    }
}

void WifiIcon::tickImpl()
{
    _animStatus = (_animStatus + 1) % 2;
}
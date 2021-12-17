#include <screen/components/Clock.h>

void Clock::drawImpl(){
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

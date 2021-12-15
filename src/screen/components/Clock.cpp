#include <screen/components/Clock.h>

void Clock::drawImpl(){
    _setDisplay(3, _x, _y);
    _display->printf("%02d%s%02d", _status.hour, _animStatus == DOTS_VISIBLE ? ":" : " ", _status.minutes);
    _setDisplay(2, _x, _y + 26);
    String s[] = {"Lun", "Mar", "Mer", "Gio", "Ven", "Sab", "Dom"};
    _display->printf("%s", s[_status.day].c_str());
}

void Clock::tickImpl(){
    _animStatus = (_animStatus + 1) % 2;
}

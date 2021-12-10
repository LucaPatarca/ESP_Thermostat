#include <screen/components/Clock.h>

void Clock::drawImpl(){
    _setDisplay(3, 0, 0);
    _display->printf("%d:%d", _status.hour, _status.minutes);
    _setDisplay(2, 0, 26);
    String s[] = {"Lun", "Mar", "Mer", "Gio", "Ven", "Sab", "Dom"};
    _display->printf("%s", s[_status.day].c_str());
}

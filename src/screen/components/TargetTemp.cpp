#include <screen/components/TargetTemp.h>

void TargetTemp::drawImpl(){
    _setDisplay(2, 0, 24);
    if (_status > 0)
        _display->printf("%.1fC", _status);
    else
        _display->printf("--.-C");
}
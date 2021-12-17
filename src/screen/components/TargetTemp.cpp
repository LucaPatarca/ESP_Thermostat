#include <screen/components/TargetTemp.h>

void TargetTemp::drawImpl(){
    _display->fillRect(_x, _y, 72, 19, BLACK);
    _setDisplay(1, _x, _y+12);
    _display->setFont(&FreeSans12pt7b);
    if (_status > 0)
        _display->printf("%.1f c", _status);
    else
        _display->printf("--.- c");
}
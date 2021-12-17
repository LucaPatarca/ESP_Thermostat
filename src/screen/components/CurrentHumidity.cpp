#include <screen/components/CurrentHumidity.h>

void CurrentHumidity::drawImpl(){
    _display->fillRect(_x, _y, 48, 18, BLACK);
    _setDisplay(1, _x, _y+12);
    _display->setFont(&FreeSans12pt7b);
    _display->printf("%.1f%%", _status);
}
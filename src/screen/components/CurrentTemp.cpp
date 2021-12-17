#include <screen/components/CurrentTemp.h>

void CurrentTemp::drawImpl(){
    _display->fillRect(_x, _y, 92, 24, BLACK);
    _setDisplay(1, _x, _y + 18);
    _display->setFont(&FreeSans18pt7b);
    _display->printf("%.1f", _status);
    _display->drawCircle(_x+72, _y+8, 4, WHITE);
    _display->setFont(&FreeSans12pt7b);
    _display->setCursor(_x+80, _y+14);
    _display->print("c");
}
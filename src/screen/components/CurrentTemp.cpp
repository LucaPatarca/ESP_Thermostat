#include <screen/components/CurrentTemp.h>
#include <Fonts/FreeSerif18pt7b.h>

void CurrentTemp::drawImpl(){
    _display->fillRect(_x, _y, 92, 24, BLACK);
    _setDisplay(1, _x, _y + 18);
    _display->setFont(&FreeSerif18pt7b);
    _display->printf("%.1f°C", _status);
}
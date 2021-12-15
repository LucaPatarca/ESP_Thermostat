#include <screen/components/CurrentHumidity.h>

void CurrentHumidity::drawImpl(){
    _setDisplay(2, _x, _y);
    _display->printf("%.1f%%", _status);
}
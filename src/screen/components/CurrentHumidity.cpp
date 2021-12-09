#include <screen/components/CurrentHumidity.h>

void CurrentHumidity::drawImpl(){
    _setDisplay(2, 0, 40);
    _display->printf("%.1f%%", _status);
}
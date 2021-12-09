#include <screen/components/CurrentTemp.h>

void CurrentTemp::drawImpl(){
    _setDisplay(3, 0, 0);
    _display->printf("%.1fC", _status);
}
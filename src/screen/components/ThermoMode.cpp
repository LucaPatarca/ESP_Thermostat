#include <screen/components/ThermoMode.h>

void ThermoMode::drawImpl(){
    String s[] = {"S", "A", "P"};
    _setDisplay(2, 111, 8);
    _display->printf("%s", s[_status].c_str());
}
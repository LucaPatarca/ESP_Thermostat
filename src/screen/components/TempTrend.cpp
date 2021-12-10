#include <screen/components/TempTrend.h>

void TempTrend::drawImpl(){
    _setDisplay(2, 111, 24);
    String s[] = {"R", "D", "S"};
    _display->print(s[_status].c_str());
}

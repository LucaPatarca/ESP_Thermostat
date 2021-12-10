#include <screen/components/BoilerState.h>

void BoilerState::drawImpl(){
    _setDisplay(2, 111, 40);
    _display->print(_status ? "*" : " ");
}

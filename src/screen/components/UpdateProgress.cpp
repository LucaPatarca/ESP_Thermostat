#include <screen/components/UpdateProgress.h>

void UpdateProgress::drawImpl(){
    _display->fillRect(_x, _y, _status, 10, WHITE);
}
#include <screen/components/UpdateProgress.h>

void UpdateProgress::drawImpl(){
    _display->fillRect(14, 40, _status, 10, WHITE);
}
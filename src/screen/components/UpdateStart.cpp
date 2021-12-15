#include <screen/components/UpdateStart.h>

void UpdateStart::drawImpl()
{
    if (_status)
    {
        _display->clearDisplay();
        _setDisplay(2, _x, _y);
        _display->println(" Updating");
        _display->drawRect(_x + 14, _y + 22, 100, 10, WHITE);

        _status = false;
    }
}
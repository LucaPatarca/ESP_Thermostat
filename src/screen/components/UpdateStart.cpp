#include <screen/components/UpdateStart.h>

void UpdateStart::drawImpl()
{
    if (_status)
    {
        _display->clearDisplay();
        _setDisplay(1, _x, _y+6);
        _display->setFont(&FreeSans12pt7b);
        _display->println("  Updating");
        _display->drawRect(_x + 14, _y + 24, 100, 10, WHITE);

        _status = false;
    }
}
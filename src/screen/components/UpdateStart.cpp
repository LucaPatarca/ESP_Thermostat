#include <screen/components/UpdateStart.h>

void UpdateStart::drawImpl()
{
    if (_status)
    {
        _display->clearDisplay();
        _setDisplay(2, 0, 18);
        _display->println(" UPDATING");
        _display->drawRect(14, 40, 100, 10, WHITE);

        _status = false;
    }
}
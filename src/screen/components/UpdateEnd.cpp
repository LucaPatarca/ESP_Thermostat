#include <screen/components/UpdateEnd.h>

void UpdateEnd::drawImpl()
{
    if (_status)
    {
        _display->clearDisplay();
        _setDisplay(1, _x, _y);
        _display->setFont(&FreeSans12pt7b);
        _display->println("    Update");
        _display->println(" Completed");

        _status = false;
    }
}
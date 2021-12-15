#include <screen/components/UpdateEnd.h>

void UpdateEnd::drawImpl()
{
    if (_status)
    {
        _display->clearDisplay();
        _setDisplay(2, _x, _y);
        _display->println("  Update");
        _display->println(" Completed");

        _status = false;
    }
}
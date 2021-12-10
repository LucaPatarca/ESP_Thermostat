#include <screen/components/UpdateEnd.h>

void UpdateEnd::drawImpl()
{
    if (_status)
    {
        _display->clearDisplay();
        _setDisplay(2, 0, 18);
        _display->println("  UPDATE");
        _display->println(" COMPLETE");

        _status = false;
    }
}
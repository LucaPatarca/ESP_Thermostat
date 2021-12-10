#include <screen/components/UpdateStatus.h>

void UpdateStatus::drawImpl(){
    switch (_status.type)
    {
    case UpdateEventType::PROGRESS:
        _display->fillRect(14, 40, (int)_status.progress, 10, WHITE);
        break;
    case UpdateEventType::START:
        _display->clearDisplay();
        _setDisplay(2, 0, 18);
        _display->println(" UPDATING");
        _display->drawRect(14, 40, 100, 10, WHITE);
        break;
    case UpdateEventType::END:
        _display->clearDisplay();
        _setDisplay(2, 0, 18);
        _display->println("  UPDATE");
        _display->println(" COMPLETE");
        break;

    default:
        break;
    }
    // TODO handle update error
}
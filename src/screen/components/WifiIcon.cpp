#include <screen/components/WifiIcon.h>

void WifiIcon::drawImpl()
{
    switch (_status)
        {
        case WiFiStatus::CONNECTING:
            _display->fillCircle(90, 50, 5, BLACK);
            _display->drawCircle(90, 50, 5, WHITE);
            break;
        case WiFiStatus::CONNECTED:
            _display->fillCircle(90, 50, 5, WHITE);
            break;
        case WiFiStatus::DISCONNECTED:
            _display->fillCircle(90, 50, 5, BLACK);

        default:
            break;
        }
}

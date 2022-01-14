#include <screen/components.h>

class TimeScreen: public Screen
{
private:
    ScreenElement *_clock;
    ScreenElement *_wifiIcon;
    ScreenElement *_thermoMode;
public:
    TimeScreen(Adafruit_SSD1306 *display);

    void wifiStatusChanged();
    void thermostatModeChanged();

    void draw() override;
    void refresh() override;
};
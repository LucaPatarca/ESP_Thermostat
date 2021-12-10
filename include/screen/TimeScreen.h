#include <screen/Screen.h>
#include <screen/components/Clock.h>
#include <WiFiListener.h>

class TimeScreen: public Screen, public WiFiListener
{
private:
    ScreenElement<Time_t> *_clock;
    ScreenElement<WiFiStatus> *_wifiIcon;
public:
    TimeScreen(Adafruit_SSD1306 *display);

    void onWiFiStatus(WiFiStatus) override;

    void draw() override;
    void refresh() override;
};
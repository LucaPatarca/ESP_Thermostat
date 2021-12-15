#include <screen/Screen.h>
#include <screen/components/Clock.h>
#include <WiFiListener.h>
#include <StateListener.h>

class TimeScreen: public Screen, public WiFiListener, public StateListener
{
private:
    ScreenElement<Time_t> *_clock;
    ScreenElement<WiFiStatus> *_wifiIcon;
    ScreenElement<Mode> *_thermoMode;
public:
    TimeScreen(Adafruit_SSD1306 *display);

    void onWiFiStatus(WiFiStatus) override;
    void onPowerState(bool) override;
    void onTargetTemperature(float) override;
    void onThermostatMode(Mode) override;
    void onSetSetting(String, String) override;

    void draw() override;
    void refresh() override;
};
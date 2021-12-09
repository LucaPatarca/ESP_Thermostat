#include "hwio.h"
#include <Wire.h>

HWIOController::HWIOController()
{
    _display = new Adafruit_SSD1306(128, 64, &Wire, -1);

    if (!_display->begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
    }
    delay(100);
}

void HWIOController::setDisplay(int size, int x, int y)
{
    _display->setTextSize(size);
    _display->setTextColor(WHITE, BLACK);
    _display->setCursor(x, y);
}

void HWIOController::onBoilerState(bool state)
{
#ifdef HWIO_DEBUG
    Serial.printf("HWIOController::onBoilerState(%s)\n", state ? "true" : "false");
#endif
    setDisplay(2, 111, 40);
    _display->print(state ? "*" : " ");
    _display->display();
}

void HWIOController::onPowerState(bool state)
{
#ifdef HWIO_DEBUG
    Serial.printf("HWIOController::onPowerState(%s)\n", state ? "true" : "false");
#endif
    setDisplay(2, 0, 24);
    if (state)
        _display->printf("%.1fC", _lastTargetTemp);
    else
        _display->printf("--.-C");
    _lastPowerState = state;
    _display->display();
}

void HWIOController::onTargetTemperature(float temp)
{
#ifdef HWIO_DEBUG
    Serial.printf("HWIOController::onTargetTemperature(%.1f)\n", temp);
#endif
    if (_lastPowerState)
    {
        setDisplay(2, 0, 24);
        _display->printf("%.1fC", temp);
    }
    _lastTargetTemp = temp;

    _display->display();
}

void HWIOController::onCurrentTemperature(Temperature_t temp)
{
#ifdef HWIO_DEBUG
    Serial.printf("HWIOController::onCurrentTemperature({%.1f, %.1f})\n", temp.temp, temp.humidity);
#endif
    setDisplay(3, 0, 0);
    _display->printf("%.1fC", temp.temp);
    setDisplay(2, 0, 40);
    _display->printf("%.1f%%", temp.humidity);
    setDisplay(2, 111, 24);
    String trend = "";
    switch (temp.trend)
    {
    case TemperatureTrend::RISE:
        trend = "R";
        break;
    case TemperatureTrend::DROP:
        trend = "D";
        break;
    case TemperatureTrend::STABLE:
        trend = "S";
        break;
    default:
        break;
    }
    _display->print(trend);
    _display->display();
}

void HWIOController::init()
{
    _display->clearDisplay();
}

void HWIOController::onWiFiStatus(WiFiStatus status)
{
    switch (status)
    {
    case WiFiStatus::CONNECTING:
#ifdef HWIO_DEBUG
        Serial.printf("HWIOController::onWiFiStatus(CONNECTING)\n");
#endif
        _display->clearDisplay();
        setDisplay(2, 0, 18);
        _display->println("CONNECTING");
        _display->println("  WIFI...");
        _display->display();
        break;
    case WiFiStatus::CONNECTED:
#ifdef HWIO_DEBUG
        Serial.printf("HWIOController::onWiFiStatus(CONNECTED)\n");
#endif
        _display->clearDisplay();
        setDisplay(2, 0, 18);
        _display->println("   WIFI");
        _display->println("CONNECTED");
        _display->display();
        break;
    case WiFiStatus::DISCONNECTED:
#ifdef HWIO_DEBUG
        Serial.printf("HWIOController::onWiFiStatus(DISCONNECTED)\n");
#endif
        _display->clearDisplay();
        setDisplay(2, 0, 18);
        _display->println("   WIFI");
        _display->println("DISCONNECT");
        _display->display();

    default:
        break;
    }
}

void HWIOController::onUpdateEvent(UpdateEvent_t event)
{
    switch (event.type)
    {
    case UpdateEventType::PROGRESS:
#ifdef HWIO_DEBUG
        Serial.printf("HWIOController::onUpdateEvent(PROGRESS)\n");
#endif
        _display->fillRect(14,40,(int)event.progress,10,WHITE);
        _display->display();
        break;
    case UpdateEventType::START:
#ifdef HWIO_DEBUG
        Serial.printf("HWIOController::onUpdateEvent(START)\n");
#endif
        _display->clearDisplay();
        setDisplay(2, 0, 18);
        _display->println(" UPDATING");
        _display->drawRect(14,40,100,10,WHITE);
        _display->display();
        break;
    case UpdateEventType::END:
#ifdef HWIO_DEBUG
        Serial.printf("HWIOController::onUpdateEvent(END)\n");
#endif
        _display->clearDisplay();
        setDisplay(2, 0, 18);
        _display->println("  UPDATE");
        _display->println(" COMPLETE");
        _display->display();
        break;

    default:
        break;
    }
    //TODO handle update error
}

void HWIOController::onThermostatMode(Mode mode)
{
    _lastMode = mode;
    String s[] = {"S", "A", "P"};
#ifdef HWIO_DEBUG
    Serial.printf("HWIOController::onThermostatMode(%s)\n", s[_lastMode].c_str());
#endif
    setDisplay(2, 111, 8);
    _display->printf("%s", s[_lastMode].c_str());

    _display->display();
}

void HWIOController::onSetSetting(String key, String value){
    //nop
}
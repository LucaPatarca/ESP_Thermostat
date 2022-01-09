#include "hwio.h"
#include <Wire.h>

HWIOController::HWIOController()
{
    _display = new Adafruit_SSD1306(128, 64, &Wire, -1);

    if (!_display->begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
    }
    _homeScreen = new HomeScreen(_display);
    _updateScreen = new UpdateScreen(_display);
    _timeScreen = new TimeScreen(_display);
    _activeScreen = _homeScreen;
    delay(100);
    _display->dim(true);
}

void HWIOController::_setActiveScreen(Screen *screen)
{
    if (_activeScreen != screen)
    {
        _display->clearDisplay();
        _activeScreen = screen;
        _activeScreen->refresh();
    }
    _lastChange = millis();
}

void HWIOController::onBoilerState(bool state)
{
#ifdef HWIO_DEBUG
    Serial.printf("HWIOController::onBoilerState(%s)\n", state ? "true" : "false");
#endif
    _homeScreen->onBoilerState(state);
}

void HWIOController::onPowerState(bool state)
{
#ifdef HWIO_DEBUG
    Serial.printf("HWIOController::onPowerState(%s)\n", state ? "true" : "false");
#endif
    _homeScreen->onPowerState(state);
}

void HWIOController::onTargetTemperature(float temp)
{
#ifdef HWIO_DEBUG
    Serial.printf("HWIOController::onTargetTemperature(%.1f)\n", temp);
#endif
    _homeScreen->onTargetTemperature(temp);
    _setActiveScreen(_homeScreen);
}

void HWIOController::onCurrentTemperature(Temperature_t temp)
{
#ifdef HWIO_DEBUG
    Serial.printf("HWIOController::onCurrentTemperature({%.1f, %.1f})\n", temp.temp, temp.humidity);
#endif
    _homeScreen->onCurrentTemperature(temp);
}

void HWIOController::init()
{
    _display->clearDisplay();
}

void HWIOController::onWiFiStatus(WiFiStatus status)
{
    _timeScreen->onWiFiStatus(status);
}

void HWIOController::onUpdateEvent(UpdateEvent_t event)
{
    if (event.type == UpdateEventType::START)
    {
#ifdef HWIO_DEBUG
        Serial.printf("HWIOController::setActiveScreen(_updateScreen)\n");
#endif
        _setActiveScreen(_updateScreen);
    }
    _updateScreen->onUpdateEvent(event);

    /*
     * needs to be called inside this method becouse during update
     * every handle() method is blocked by the update operation
     */
    _activeScreen->draw();

    _display->display();
}

void HWIOController::onThermostatMode(Mode mode)
{
    _timeScreen->onThermostatMode(mode);
    _setActiveScreen(_timeScreen);
}

void HWIOController::onSetSetting(String key, String value)
{
    // nop
}

void HWIOController::handle()
{
    if (millis() > _lastChange + SCREEN_INTERVAL)
    {
#ifdef HWIO_DEBUG
        Serial.printf("HWIOController::changeScreen()\n");
#endif
        if (_activeScreen == _homeScreen)
            _setActiveScreen(_timeScreen);
        else
            _setActiveScreen(_homeScreen);
    }
    _activeScreen->draw();

    _display->display();
}
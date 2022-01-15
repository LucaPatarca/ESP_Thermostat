#include "hwio.h"
#include <Wire.h>

HWIOController::HWIOController()
{
    m_display = new Adafruit_SSD1306(128, 64, &Wire, -1);

    if (!m_display->begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
    }
    m_homeScreen = new HomeScreen(m_display);
    m_updateScreen = new UpdateScreen(m_display);
    m_timeScreen = new TimeScreen(m_display);
    m_activeScreen = m_homeScreen;
    delay(100);
    m_display->dim(true);
}

void HWIOController::_setActiveScreen(Screen *screen)
{
    if (m_activeScreen != screen)
    {
        m_display->clearDisplay();
        m_activeScreen = screen;
        m_activeScreen->refresh();
    }
    m_lastChange = millis();
}

void HWIOController::boilerStateChanged()
{
#ifdef HWIO_DEBUG
    Serial.printf("HWIOController::onBoilerState(%s)\n", state ? "true" : "false");
#endif
    m_homeScreen->boilerStateChanged();
}

void HWIOController::powerStateChanged()
{
#ifdef HWIO_DEBUG
    Serial.printf("HWIOController::onPowerState(%s)\n", state ? "true" : "false");
#endif
    m_homeScreen->powerStateChanged();
}

void HWIOController::targetTemperatureChanged()
{
#ifdef HWIO_DEBUG
    Serial.printf("HWIOController::onTargetTemperature(%.1f)\n", temp);
#endif
    m_homeScreen->targetTemperatureChanged();
    //TODO check if it was manual
    _setActiveScreen(m_homeScreen);
}

void HWIOController::currentTemperatureChanged()
{
#ifdef HWIO_DEBUG
    Serial.printf("HWIOController::onCurrentTemperature({%.1f, %.1f})\n", temp.temp, temp.humidity);
#endif
    m_homeScreen->currentTemperatureChanged();
}

void HWIOController::init()
{
    m_display->clearDisplay();
    m_display->display();
}

void HWIOController::wifiStatusChanged()
{
    m_timeScreen->wifiStatusChanged();
}

void HWIOController::onUpdateEvent(const UpdateEvent_t& event)
{
    if (event.type == UpdateEventType::START)
    {
#ifdef HWIO_DEBUG
        Serial.printf("HWIOController::setActiveScreen(m_updateScreen)\n");
#endif
        _setActiveScreen(m_updateScreen);
    }
    m_updateScreen->onUpdateEvent(event);
    m_display->display();
}

void HWIOController::thermostatModeChanged()
{
    m_timeScreen->thermostatModeChanged();
    //TODO check if it was manual
    _setActiveScreen(m_timeScreen);
}

void HWIOController::handle()
{
    if (millis() > m_lastChange + SCREEN_INTERVAL)
    {
#ifdef HWIO_DEBUG
        Serial.printf("HWIOController::changeScreen()\n");
#endif
        if (m_activeScreen == m_homeScreen)
            _setActiveScreen(m_timeScreen);
        else
            _setActiveScreen(m_homeScreen);
    }
    m_activeScreen->draw();

    m_display->display();
}
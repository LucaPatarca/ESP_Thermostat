#include "display.h"
#include <Wire.h>

#define SCREEN_INTERVAL         10000   //in milliseconds
#define NOTIFICATION_INTERVAL   8000    //in milliseconds

DisplayController::DisplayController()
    : m_display(Adafruit_SSD1306(128, 64, &Wire, -1)),
    m_homeScreen(HomeScreen(m_display)),
    m_updateScreen(UpdateScreen(m_display)),
    m_timeScreen(TimeScreen(m_display)),
    m_notificationScreen(NotificationScreen(m_display))
{
    if (!m_display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
    }
    delay(100);
    m_display.dim(true);
    _setActiveScreen(&m_homeScreen);
}

void DisplayController::_setActiveScreen(Screen *screen)
{
    if (m_activeScreen != screen)
    {
        m_changeInterval = SCREEN_INTERVAL;
        m_display.clearDisplay();
        m_activeScreen = screen;
        m_activeScreen->refresh();
    }
    m_lastChange = millis();
}

void DisplayController::_notify(const String& text){
    m_changeInterval = NOTIFICATION_INTERVAL;
    m_display.clearDisplay();
    m_notificationScreen.setText(text);
    m_activeScreen = &m_notificationScreen;
    m_activeScreen->refresh();
    m_lastChange = millis();
}

void DisplayController::boilerStateChanged()
{
#ifdef HWIO_DEBUG
    Serial.printf("DisplayController::onBoilerState(%s)\n", state ? "true" : "false");
#endif
    m_homeScreen.boilerStateChanged();
}

void DisplayController::powerStateChanged()
{
#ifdef HWIO_DEBUG
    Serial.printf("DisplayController::onPowerState(%s)\n", state ? "true" : "false");
#endif
    m_homeScreen.powerStateChanged();
}

void DisplayController::targetTemperatureChanged(Cause cause)
{
#ifdef HWIO_DEBUG
    Serial.printf("DisplayController::onTargetTemperature(%.1f)\n", temp);
#endif
    m_homeScreen.targetTemperatureChanged();
    if(cause == Cause::MANUAL)
        _setActiveScreen(&m_homeScreen);
}

void DisplayController::currentTemperatureChanged()
{
#ifdef HWIO_DEBUG
    Serial.printf("DisplayController::onCurrentTemperature({%.1f, %.1f})\n", temp.temp, temp.humidity);
#endif
    m_homeScreen.currentTemperatureChanged();
}

void DisplayController::wifiStatusChanged()
{
    m_timeScreen.wifiStatusChanged();
}

void DisplayController::onUpdateEvent(const UpdateEvent_t& event)
{
    if (event.type == UpdateEventType::START)
    {
#ifdef HWIO_DEBUG
        Serial.printf("DisplayController::setActiveScreen(m_updateScreen)\n");
#endif
        _setActiveScreen(&m_updateScreen);
    }
    m_updateScreen.onUpdateEvent(event);
    m_display.display();
}

void DisplayController::thermostatModeChanged(Cause cause)
{
    m_timeScreen.thermostatModeChanged();
    if(cause == Cause::MANUAL)
        _setActiveScreen(&m_timeScreen);
}

void DisplayController::handle()
{
    if (millis() > m_lastChange + m_changeInterval)
    {
#ifdef HWIO_DEBUG
        Serial.printf("DisplayController::changeScreen()\n");
#endif
        if (m_activeScreen == &m_homeScreen)
            _setActiveScreen(&m_timeScreen);
        else
            _setActiveScreen(&m_homeScreen);
    }
    m_activeScreen->draw();

    m_display.display();
}
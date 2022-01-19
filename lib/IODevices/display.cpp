#ifndef NO_DISPLAY

#include <display.h>
#include <Wire.h>
#include <sdebug.h>

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
        ERROR("SSD1306 allocation failed");
    }
    delay(100);
    m_display.dim(true);
    _setActiveScreen(&m_homeScreen);
    FINE("display initialized");
}

void DisplayController::_setActiveScreen(Screen *screen)
{
    if (m_activeScreen != screen)
    {
        FINE("changing screen");
        m_changeInterval = SCREEN_INTERVAL;
        m_display.clearDisplay();
        m_activeScreen = screen;
        m_activeScreen->refresh();
    }
    m_lastChange = millis();
}

void DisplayController::_notify(const String& text){
    INFO("displayng notification %s", text.c_str());
    m_changeInterval = NOTIFICATION_INTERVAL;
    m_display.clearDisplay();
    m_notificationScreen.setText(text);
    m_activeScreen = &m_notificationScreen;
    m_activeScreen->refresh();
    m_lastChange = millis();
}

void DisplayController::boilerStateChanged()
{
    m_homeScreen.boilerStateChanged();
}

void DisplayController::powerStateChanged()
{
    m_homeScreen.powerStateChanged();
}

void DisplayController::targetTemperatureChanged(Cause cause)
{
    m_homeScreen.targetTemperatureChanged();
    if(cause == Cause::MANUAL)
        _setActiveScreen(&m_homeScreen);
}

void DisplayController::currentTemperatureChanged()
{
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
        if (m_activeScreen == &m_homeScreen)
            _setActiveScreen(&m_timeScreen);
        else
            _setActiveScreen(&m_homeScreen);
    }
    m_activeScreen->draw();

    m_display.display();
}

#endif
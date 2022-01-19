#ifndef NO_DISPLAY

#include <screen/UpdateScreen.h>
#include <listeners.h>

UpdateScreen::UpdateScreen(Adafruit_SSD1306 &display) : Screen(display)
{
}

void UpdateScreen::onUpdateEvent(const UpdateEvent_t& event)
{
    switch (event.type)
    {
    case UpdateEventType::START:
        m_display.clearDisplay();
        m_display.setTextSize(1);
        m_display.setTextColor(WHITE, BLACK);
        m_display.setCursor(0, 24);
        m_display.setFont(&FreeSans12pt7b);
        m_display.println("  Updating");
        m_display.drawRect(14, 42, 100, 12, WHITE);
        break;
    case UpdateEventType::PROGRESS:
        m_display.fillRect(14, 42, static_cast<int>(event.progress), 12, WHITE);
        break;
    case UpdateEventType::END:
        m_display.clearDisplay();
        m_display.setTextSize(1);
        m_display.setTextColor(WHITE, BLACK);
        m_display.setCursor(0, 18);
        m_display.setFont(&FreeSans12pt7b);
        m_display.println("    Update");
        m_display.println(" Completed");
        break;
    //TODO handle errors
    default:
        break;
    }
}

void UpdateScreen::draw()
{
    //the drawing is done inside the onUpdateEvent function
}

void UpdateScreen::refresh()
{
    //nothing to do here
}

#endif
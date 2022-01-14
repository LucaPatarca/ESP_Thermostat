#include <screen/UpdateScreen.h>
#include <listeners.h>

UpdateScreen::UpdateScreen(Adafruit_SSD1306 *display) : Screen(display)
{
}

void UpdateScreen::onUpdateEvent(UpdateEvent_t& event)
{
    switch (event.type)
    {
    case UpdateEventType::START:
        _display->clearDisplay();
        _display->setTextSize(1);
        _display->setTextColor(WHITE, BLACK);
        _display->setCursor(0, 24);
        _display->setFont(&FreeSans12pt7b);
        _display->println("  Updating");
        _display->drawRect(14, 42, 100, 10, WHITE);
        break;
    case UpdateEventType::PROGRESS:
        _display->fillRect(14, 18, (int)event.progress, 12, WHITE);
        break;
    case UpdateEventType::END:
        _display->clearDisplay();
        _display->setTextSize(1);
        _display->setTextColor(WHITE, BLACK);
        _display->setCursor(0, 18);
        _display->setFont(&FreeSans12pt7b);
        _display->println("    Update");
        _display->println(" Completed");
        break;

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
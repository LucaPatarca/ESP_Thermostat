#ifndef NO_DISPLAY

#include <screen/NotificationScreen.h>

void NotificationScreen::setText(const String &text)
{
    m_text = text;
    m_needsUpdate = true;
}

void NotificationScreen::draw()
{
    if (m_needsUpdate)
    {
        m_display.setTextSize(1);
        m_display.setTextColor(WHITE, BLACK);
        m_display.setCursor(0, 0);
        m_display.setFont(&FreeSans12pt7b);
        m_display.printf("%s\n", m_text.c_str());
    }
}

void NotificationScreen::refresh()
{
    m_needsUpdate = true;
}

#endif
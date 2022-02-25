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
        int y = 0;
        if (m_text.length() > 25)
        {
            m_display.setFont(&FreeSans9pt7b);
            y=12;
        }
        else
        {
            m_display.setFont(&FreeSans12pt7b);
            y=18;
        }
        m_display.setTextSize(1);
        m_display.setTextColor(WHITE, BLACK);
        m_display.setCursor(0, y);
        m_display.printf("%s\n", m_text.c_str());
    }
}

void NotificationScreen::refresh()
{
    m_needsUpdate = true;
}

#endif
#include "ntime.h"

TimeController::TimeController()
{
#ifdef NTIME_DEBUG
    Serial.printf("[TimeController] created\n");
#endif
    m_client = new NTPClient(m_udp, "pool.ntp.org", UTC_OFFSET);
}

void TimeController::begin()
{
#ifdef NTIME_DEBUG
    Serial.printf("[TimeController] begin\n");
#endif
    m_client->begin();
    m_client->update();
}

int TimeController::getDayOfWeek()
{
    checkUpdate();
    int day = (m_client->getDay() + 6) % 7;
#ifdef NTIME_DEBUG
    Serial.printf("[TimeController] get day %d\n", day);
#endif
    return day;
}

int TimeController::getTime()
{
    checkUpdate();
    int hour = m_client->getHours() * 2;
    int minutes = m_client->getMinutes();
    if (minutes >= 30)
        hour += 1;

#ifdef NTIME_DEBUG
    Serial.printf("[TimeController] get time %d\n", hour);
#endif
    return hour;
}

void TimeController::checkUpdate()
{
    if (millis() - _lastUpdate > UPDATE_INTERVAL)
    {
#ifdef NTIME_DEBUG
        Serial.printf("[TimeController] update\n");
#endif
        m_client->update();
        _lastUpdate = millis();
    }
}

String TimeController::getFormattedTime()
{
    return m_client->getFormattedTime();
}
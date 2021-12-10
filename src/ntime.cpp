#include <ntime.h>

TimeController::TimeController()
{
#ifdef NTIME_DEBUG
    Serial.printf("[TimeController] created\n");
#endif
    m_client = new NTPClient(m_udp, "pool.ntp.org", UTC_OFFSET);
    m_client->begin();
}

void TimeController::begin()
{
// #ifdef NTIME_DEBUG
//     Serial.printf("[TimeController] begin\n");
// #endif
//     m_client->begin();
//     m_client->update();
}

Time_t TimeController::getTime()
{
    checkUpdate();
    int day = (m_client->getDay() + 6) % 7;
    int hour = m_client->getHours();
    int min = m_client->getMinutes();
    return {hour, min, day};
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
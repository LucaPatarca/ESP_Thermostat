#include <ntime.h>

TimeController::TimeController()
{
#ifdef NTIME_DEBUG
    Serial.printf("[TimeController] created\n");
#endif
    m_client = new NTPClient(m_udp, "pool.ntp.org", UTC_OFFSET, UPDATE_INTERVAL);
    m_client->begin();
}

Time_t TimeController::getTime()
{
    m_client->update();
    int day = (m_client->getDay() + 6) % 7;
    int hour = m_client->getHours();
    int min = m_client->getMinutes();
    return {hour, min, day};
}

String TimeController::getFormattedTime()
{
    return m_client->getFormattedTime();
}
#include "ntime.h"

TimeClass::TimeClass(){
    m_client = new NTPClient(m_udp, "pool.ntp.org", UTC_OFFSET);
}

void TimeClass::begin(){
    m_client->begin();
    m_client->update();
}

WeekDay TimeClass::getDayOfWeek(){
    checkUpdate();
    int day = m_client->getDay()-1;
    return static_cast<WeekDay>(day);
}

int TimeClass::getHour(){
    checkUpdate();
    return m_client->getHours();
}

void TimeClass::checkUpdate(){
    if(millis() > m_updateTime){
        m_client->update();
        m_updateTime = millis()+UPDATE_INTERVAL;
    }
}

TimeClass Time;
#include "ntime.h"

TimeController::TimeController(){
    m_client = new NTPClient(m_udp, "pool.ntp.org", UTC_OFFSET);
}

void TimeController::begin(){
    m_client->begin();
    m_client->update();
}

int TimeController::getDayOfWeek(){
    checkUpdate();
    int day = (m_client->getDay() + 6)%7;
    return day;
}

int TimeController::getTime(){
    checkUpdate();
    int hour = m_client->getHours()*2;
    int minutes = m_client->getMinutes();
    if(minutes>=30)
        hour+=1;
    return hour;
}

void TimeController::checkUpdate(){
    if(millis()-_lastUpdate > UPDATE_INTERVAL){
        m_client->update();
        _lastUpdate = millis();
    }
}

String TimeController::getFormattedTime(){
    return m_client->getFormattedTime();
}
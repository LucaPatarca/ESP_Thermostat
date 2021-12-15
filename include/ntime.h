#pragma once

#include <WiFiUdp.h>
#include <NTPClient.h>

#define UTC_OFFSET 3600
#define UPDATE_INTERVAL 1800000  //update every 30 minutes

// #define NTIME_DEBUG

typedef struct Time{
    int hour;
    int minutes;
    int day;
    bool operator !=(Time &rhs) const {
        return this->hour != rhs.hour && this->minutes != rhs.minutes && this->day != rhs.day;
    }
} Time_t;

class TimeController{
    private:
        WiFiUDP m_udp;
        NTPClient *m_client;
        unsigned long _lastUpdate;

    public:
        TimeController();
        Time_t getTime();
        String getFormattedTime();
};

extern TimeController *Time;
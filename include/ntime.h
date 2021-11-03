#pragma once

#include <WiFiUdp.h>
#include <NTPClient.h>

#define UTC_OFFSET 7200
#define UPDATE_INTERVAL 1800000  //update every 30 minutes

#ifdef ENABLE_DEBUG
#define NTIME_DEBUG
#endif

enum WeekDay{
    LUNEDI,
    MARTEDI,
    MERCOLEDI,
    GIOVEDI,
    VENERDI,
    SABATO,
    DOMENICA
};

class TimeClass{
    private:
        WiFiUDP m_udp;
        NTPClient *m_client;
        unsigned long m_updateTime;
        void checkUpdate();

    public:
        TimeClass();
        void begin();
        WeekDay getDayOfWeek();
        int getHour();
};

extern TimeClass Time;
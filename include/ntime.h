#pragma once

#include <WiFiUdp.h>
#include <NTPClient.h>

#define UTC_OFFSET 7200
#define UPDATE_INTERVAL 1800000  //update every 30 minutes

// #define NTIME_DEBUG

class TimeController{
    private:
        WiFiUDP m_udp;
        NTPClient *m_client;
        unsigned long _lastUpdate;
        void checkUpdate();

    public:
        TimeController();
        void begin();
        int getDayOfWeek();
        int getTime();
};
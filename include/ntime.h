#pragma once

#include <WiFiUdp.h>
#include <NTPClient.h>

#define UTC_OFFSET 3600
#define UPDATE_INTERVAL 1000 //update every second

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
        String getFormattedTime();
};
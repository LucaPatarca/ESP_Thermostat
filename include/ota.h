#pragma once

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <state.h>
#include <listeners.h>

#define OTA_EVENT_INTERVAL 100

// #define OTA_DEBUG

class OTAController
{
private:
    State &m_state;
    unsigned long _updateTime;
    void(*onUpdateEvent)(UpdateEvent_t&);

    OTAController();
    void connect();
public:
    OTAController(OTAController&) = delete;

    static OTAController& Instance(){
        static OTAController controller;
        return controller;
    }

    void wifiStatusChanged();

    void handle();
    void setOnUpdateEvent(void(*)(UpdateEvent_t&));
};
#pragma once

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <state.h>
#include <listeners.h>
#include <functional>

#define OTA_EVENT_INTERVAL 100

// #define OTA_DEBUG

class OTAController
{
private:
    State &m_state;
    unsigned long _updateTime;
    std::function<void(const UpdateEvent_t&)> onUpdateEvent;

    OTAController();
    void connect();
public:
    OTAController(const OTAController&) = delete;

    static OTAController& Instance(){
        static OTAController controller;
        return controller;
    }

    void wifiStatusChanged();

    void handle();
    void setOnUpdateEvent(std::function<void(const UpdateEvent_t&)>);
};
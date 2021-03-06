#pragma once

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <state.h>
#include <listeners.h>
#include <functional>

class OTAController
{
private:
    State &m_state;
    unsigned long m_updateTime;
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
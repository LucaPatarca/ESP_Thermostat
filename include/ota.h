#pragma once

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <EventEmitter.h>
#include <UpdateListener.h>

#define OTA_EVENT_INTERVAL 500

class OTAController : public EventEmitter<UpdateListener>
{
private:
    unsigned long _updateTime;

public:
    OTAController();
    void connect();
    void handle();
};
#pragma once

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <EventEmitter.h>
#include <UpdateListener.h>
#include <WiFiListener.h>

#define OTA_EVENT_INTERVAL 100

// #define OTA_DEBUG

class OTAController : public EventEmitter<UpdateListener>, public WiFiListener
{
private:
    unsigned long _updateTime;
    bool _isConnected;
public:
    OTAController();

    void onWiFiStatus(WiFiStatus status) override;

    void connect();
    void handle();
};
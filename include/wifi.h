#pragma once

#include <EventEmitter.h>
#include <WiFiListener.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

// #define WIFI_DEBUG

class WifiController : public EventEmitter<WiFiListener>
{
private:
    void notifiStatus(WiFiStatus);

    void onWiFiConnect(const WiFiEventStationModeGotIP&);
    void onWiFiDisconnect(const WiFiEventStationModeDisconnected&);
public:
    void connect();
};
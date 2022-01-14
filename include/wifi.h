#pragma once

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

#include <state.h>

// #define WIFI_DEBUG

class WifiController
{
private:
    bool _firstConnection;
    WiFiEventHandler wifiConnectHandler;
    WiFiEventHandler wifiDisconnectHandler;

    void notifiStatus(WiFiStatus);

    void onWiFiConnect(const WiFiEventStationModeGotIP&);
    void onWiFiDisconnect(const WiFiEventStationModeDisconnected&);

    WifiController();
public:
    WifiController(WifiController&) = delete;

    static WifiController& Instance(){
        static WifiController controller;
        return controller;
    }
    
    void connect();
};
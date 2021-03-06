#pragma once

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

#include <state.h>
#include <ping.h>

class WifiController
{
private:
    bool m_firstConnection;
    unsigned long m_lastCheck;
    WiFiEventHandler m_wifiConnectHandler;
    WiFiEventHandler m_wifiDisconnectHandler;
    WiFiUDP m_udp;
    ping_option m_options;
    ESP8266WebServer m_server;
    String *m_token;

    void notifiStatus(WiFiStatus);

    void onWiFiConnect(const WiFiEventStationModeGotIP&);
    void onWiFiDisconnect(const WiFiEventStationModeDisconnected&);

    static void onPingReceive(void *opt, void *resp);

    void onRecvWifiCredentials();
    void onRecvApiCredentials();

    String* generateToken();

    WifiController();
public:
    WifiController(WifiController&) = delete;

    static WifiController& Instance(){
        static WifiController controller;
        return controller;
    }
    
    void connect();
    void handle();
};
#pragma once

#include <EventEmitter.h>
#include <WiFiListener.h>

// #define WIFI_DEBUG

class WifiController : public EventEmitter<WiFiListener>
{
private:
    void notifiStatus(WiFiStatus);
public:
    void connect();
};
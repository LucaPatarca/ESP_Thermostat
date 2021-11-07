#pragma once

#include <EventEmitter.h>
#include <WiFiListener.h>

#ifdef ENABLE_DEBUG
#define WIFI_DEBUG
#endif

class WifiController : public EventEmitter<WiFiListener>
{
private:
    void notifiStatus(WiFiStatus);
public:
    void connect();
};
#pragma once

enum WiFiStatus{
    CONNECTED,
    DISCONNECTED,
    CONNECTING
};

class WiFiListener
{
public:
    virtual void onWiFiStatus(WiFiStatus) = 0;
};
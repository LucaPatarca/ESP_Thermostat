#pragma once

#ifdef ENABLE_DEBUG
#define WIFI_DEBUG
#endif

class WifiController{
    public:
        void connect();
        bool isConnected();
};
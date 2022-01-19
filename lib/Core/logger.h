#ifdef ENABLE_REMOTE_LOGGING

#pragma once

#include <ESP8266HTTPClient.h>

class RemoteLogger
{
private:
    WiFiClient m_client;
    HTTPClient m_http;

    RemoteLogger();

public:
    RemoteLogger(RemoteLogger&) = delete;

    static RemoteLogger& Instance(){
        static RemoteLogger logger;
        return logger;
    }

    void sendLog();
};

#define SEND_REMOTE_LOG RemoteLogger::Instance().sendLog()

#else
#define SEND_REMOTE_LOG
#endif
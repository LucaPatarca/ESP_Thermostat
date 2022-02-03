#ifdef ENABLE_REMOTE_LOGGING

#include <logger.h>
#include <state.h>
#include <sdebug.h>

RemoteLogger::RemoteLogger() {}

void RemoteLogger::sendLog()
{
    if (!m_http.begin(m_client, "http://192.168.5.99:9000/log/"))
    {
        ERROR("cannot initialize http client");
        return;
    }
    State &state = State::Instance();
    m_http.addHeader("Content-Type", "application/json");
    char json[512];
    sprintf(json, "{\"temp\":%.2f,\"humidity\":%.2f,\"trend\":\"%s\",\"coeff\":%.4f,\"power\":%s,\"target\":%.2f,\"boiler\":%s}",
            state.getCurrentTemperature().temp,
            state.getCurrentTemperature().humidity,
            tempTrendNames[state.getCurrentTemperature().trend][0],
            state.getCurrentTemperature().coefficient,
            state.getPowerState() ? "true" : "false",
            state.getTargetTemperature(),
            state.getBoilerState() ? "true" : "false");
    int code = m_http.POST(json);
    if (code == 200)
    {
        INFO("remote logger sent: %s", json);
    }
    else if (code > 0)
    {
        WARN("server responded with error code: %d", code);
    }
    else
    {
        ERROR("cannot send to server: %d", code);
    }
}

#endif
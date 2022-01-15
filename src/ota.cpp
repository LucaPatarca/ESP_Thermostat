#include <ota.h>

OTAController::OTAController()
    : m_state(State::Instance())
{
    ArduinoOTA.onStart([this]()
                       {
                           UpdateEvent_t event = UpdateEvent_t{UpdateEventType::START, 0, ""};
                           onUpdateEvent(event);
                       });
    ArduinoOTA.onEnd([this]()
                     {
                         UpdateEvent_t event = UpdateEvent_t{UpdateEventType::END, 100, ""};
                         onUpdateEvent(event);
                     });
    ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total)
                          {
                              if (millis() > _updateTime)
                              {
                                  UpdateEvent_t event = UpdateEvent_t{UpdateEventType::PROGRESS, ((float)progress / ((float)total / 100)), ""};
                                  onUpdateEvent(event);
                                  _updateTime = millis() + OTA_EVENT_INTERVAL;
                              }
                          });
    ArduinoOTA.onError([this](ota_error_t error)
                       {
                           const char *message;
                           if (error == OTA_AUTH_ERROR)
                               message = "Auth Failed";
                           else if (error == OTA_BEGIN_ERROR)
                               message = "Begin Failed";
                           else if (error == OTA_CONNECT_ERROR)
                               message = "Connect Failed";
                           else if (error == OTA_RECEIVE_ERROR)
                               message = "Receive Failed";
                           else if (error == OTA_END_ERROR)
                               message = "End Failed";
                           else
                               message = "Unknown";
                           UpdateEvent_t event = UpdateEvent_t{UpdateEventType::ERROR, -1, message};
                           onUpdateEvent(event);
                       });
}

void OTAController::wifiStatusChanged()
{
    if (m_state.getWifiStatus() == WiFiStatus::CONNECTED)
    {
        connect();
    }
}

void OTAController::connect()
{
#ifdef OTA_DEBUG
    Serial.printf("[OTAController] connecting...\n");
#endif
    ArduinoOTA.begin();
#ifdef OTA_DEBUG
    Serial.printf("[OTAController] connected\n");
#endif
}

void OTAController::handle()
{
    if (m_state.getWifiStatus() == WiFiStatus::CONNECTED)
    {
        ArduinoOTA.handle();
    }
}

void OTAController::setOnUpdateEvent(std::function<void(const UpdateEvent_t&)> func)
{
    onUpdateEvent = func;
}
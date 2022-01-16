#include <ota.h>
#include <sdebug.h>

#define OTA_EVENT_INTERVAL 100

OTAController::OTAController()
    : m_state(State::Instance())
{
    ArduinoOTA.onStart([this]()
                       {
                           INFO("update started");
                           onUpdateEvent(UpdateEvent_t{UpdateEventType::START, 0, ""});
                       });
    ArduinoOTA.onEnd([this]()
                     {
                         INFO("update completed");
                         onUpdateEvent(UpdateEvent_t{UpdateEventType::END, 100, ""});
                     });
    ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total)
                          {
                              if (millis() > m_updateTime)
                              {
                                  float percent = ((float)progress / ((float)total / 100));
                                  FINE("update progress %.2f", percent);
                                  onUpdateEvent(UpdateEvent_t{UpdateEventType::PROGRESS, percent, ""});
                                  m_updateTime = millis() + OTA_EVENT_INTERVAL;
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
                            ERROR("update error: %s", message);
                            onUpdateEvent(UpdateEvent_t{UpdateEventType::ERROR, -1, message});
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
    INFO("ota connecting...");
    ArduinoOTA.begin();
    INFO("ota connected");
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
#include <ota.h>
#include <sdebug.h>

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
                              if (millis() > _updateTime)
                              {
                                  float percent = ((float)progress / ((float)total / 100));
                                  FINFO("update progress %.2f", percent);
                                  onUpdateEvent(UpdateEvent_t{UpdateEventType::PROGRESS, percent, ""});
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
                            FERROR("update error: %s", message);
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
    INFO("connecting...");
    ArduinoOTA.begin();
    INFO("connected");
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
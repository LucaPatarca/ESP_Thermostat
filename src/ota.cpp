#include <ota.h>

OTAController::OTAController()
{
    ArduinoOTA.onStart([this]()
                       {
                           for (UpdateListener *listener : _listeners)
                           {
                               listener->onUpdateEvent({UpdateEventType::START, 0, ""});
                           }
                       });
    ArduinoOTA.onEnd([this]()
                     {
                         for (UpdateListener *listener : _listeners)
                         {
                             listener->onUpdateEvent({UpdateEventType::END, 100, ""});
                         }
                     });
    ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total)
                          {
                              if (millis() > _updateTime)
                              {
                                  for (UpdateListener *listener : _listeners)
                                  {
                                      listener->onUpdateEvent({UpdateEventType::PROGRESS, ((float)progress / ((float)total / 100)), ""});
                                  }
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
                           for (UpdateListener *listener : _listeners)
                           {
                               listener->onUpdateEvent({UpdateEventType::ERROR, -1, message});
                           }
                       });
}

void OTAController::connect()
{
    ArduinoOTA.begin();
}

void OTAController::handle()
{
    ArduinoOTA.handle();
}
#include <ota.h>

OTAController::OTAController(AlexaController *alexa, HWIOController *hwio)
{
    m_alexa = alexa;
    m_hwio = hwio;

    ArduinoOTA.onStart([this]()
                       {
                           Serial.println("Start update");
                           m_alexa->stop();
                       });
    ArduinoOTA.onEnd([this]()
                     { 
                         Serial.println("\nEnd update"); 
                         m_hwio->updateComplete();
                     });
    ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total)
                          {
                              Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
                              m_hwio->updateProgress(progress / (total / 100));
                          });
    ArduinoOTA.onError([](ota_error_t error)
                       {
                           Serial.printf("Error[%u]: ", error);
                           if (error == OTA_AUTH_ERROR)
                               Serial.println("Auth Failed");
                           else if (error == OTA_BEGIN_ERROR)
                               Serial.println("Begin Failed");
                           else if (error == OTA_CONNECT_ERROR)
                               Serial.println("Connect Failed");
                           else if (error == OTA_RECEIVE_ERROR)
                               Serial.println("Receive Failed");
                           else if (error == OTA_END_ERROR)
                               Serial.println("End Failed");
                       });
    ArduinoOTA.begin();
}

void OTAController::handle()
{
    ArduinoOTA.handle();
}
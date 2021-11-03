#pragma once

#include <alexa.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <hwio.h>

class OTAController{
    private:
        AlexaController *m_alexa;
        HWIOController *m_hwio;
    public:
        OTAController(AlexaController*, HWIOController*);
        void handle();
};
#pragma once

#include <state.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define MSG_BUFFER_SIZE 64

class MQTTController {
public:
  MQTTController(const MQTTController &) = delete;

  static MQTTController &Instance() {
    static MQTTController instance;
    return instance;
  }

  void targetTemperatureChanged(Cause);
  void powerStateChanged(Cause);
  void currentTemperatureChanged();
  void thermostatModeChanged(Cause);

  void connect();
  void handle();

private:
  MQTTController();
  String convertPayloadToStr(byte*, unsigned int);
  void MQTTCallback(char*, byte*, unsigned int);

  State &m_state;
  WiFiClient m_wifiClient;
  PubSubClient m_mqttClient;
  unsigned long m_lastConnectionAttempt;
  char m_msgBuf[MSG_BUFFER_SIZE];
};

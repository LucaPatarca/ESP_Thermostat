#include "SinricProUDP.h"
#include "SinricProConfig.h"
#include "SinricProDebug.h"

#if defined ESP8266
  #include <ESP8266WiFi.h>
#endif
#if defined ESP32
  #include <WiFi.h>
#endif

namespace SINRICPRO_NAMESPACE{

void UdpListener::begin(SinricProQueue_t* receiveQueue) {
  this->receiveQueue = receiveQueue;
  #if defined ESP8266
    _udp.beginMulticast(WiFi.localIP(), UDP_MULTICAST_IP, UDP_MULTICAST_PORT);
  #endif  
  #if defined ESP32
    _udp.beginMulticast(UDP_MULTICAST_IP, UDP_MULTICAST_PORT);
  #endif  
}

void UdpListener::handle() {
  int len = _udp.parsePacket();
  if (!len) return;

  if (len) {
    char* buf = (char*) malloc(len+1);
    memset(buf, 0, len+1);
    _udp.read(buf, len);
    SinricProMessage* request = new SinricProMessage(IF_UDP, buf);
    DEBUG_SINRIC("[SinricPro:UDP]: receiving request\r\n%s\r\n", buf);
    free(buf);
    receiveQueue->push(request);
  }
}

void UdpListener::sendMessage(String &message) {
  _udp.beginPacket(_udp.remoteIP(), _udp.remotePort());
  _udp.print(message);
  _udp.endPacket();
  // restart UDP??
  #if defined ESP8266
    _udp.beginMulticast(WiFi.localIP(), UDP_MULTICAST_IP, UDP_MULTICAST_PORT);
  #endif  
  #if defined ESP32
    _udp.beginMulticast(UDP_MULTICAST_IP, UDP_MULTICAST_PORT);
  #endif  
}

void UdpListener::stop() {
  _udp.stop();
}
}
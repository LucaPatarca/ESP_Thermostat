#include "SinricProWebsocket.h"

namespace SINRICPRO_NAMESPACE{

WebsocketListener::WebsocketListener() 
: _begin(false)
, _isConnected(false)
, restoreDeviceStates(false)
, _wsConnectedCb(nullptr)
, _wsDisconnectedCb(nullptr) {}

WebsocketListener::~WebsocketListener() {
  stop();
}

AdvWebSocketsClient::AdvWebSocketsClient()
: _rttCb(nullptr) {}

void AdvWebSocketsClient::onPong(std::function<void(uint32_t)> cb) { 
  _rttCb = cb; 
}

void AdvWebSocketsClient::messageReceived(WSclient_t * client, WSopcode_t opcode, uint8_t * payload, size_t length, bool fin) {
  if ( (opcode == WSop_pong) && (_rttCb) ) _rttCb( millis() - _client.lastPing );
  WebSocketsClient::messageReceived(client, opcode, payload, length, fin);
}

void WebsocketListener::setExtraHeaders() {
  String headers  = "appkey:" + appKey + "\r\n";
         headers += "deviceids:" + deviceIds + "\r\n";
         headers += "restoredevicestates:" + String(restoreDeviceStates?"true":"false") + "\r\n";
         headers += "ip:" + WiFi.localIP().toString() + "\r\n";
         headers += "mac:" + WiFi.macAddress() + "\r\n";
  #ifdef ESP8266
         headers += "platform:ESP8266\r\n";
  #endif
  #ifdef ESP32
         headers += "platform:ESP32\r\n";
  #endif
         headers += "version:" + String(SINRICPRO_VERSION);
  DEBUG_SINRIC("[SinricPro:Websocket]: headers: \r\n%s\r\n", headers.c_str());
  webSocket.setExtraHeaders(headers.c_str());
}

void WebsocketListener::begin(String server, String appKey, String deviceIds, SinricProQueue_t* receiveQueue) {
  if (_begin) return;
  _begin = true;

  this->receiveQueue = receiveQueue;
  this->appKey = appKey;
  this->deviceIds = deviceIds;

#ifdef WEBSOCKET_SSL
  DEBUG_SINRIC("[SinricPro:Websocket]: Connecting to WebSocket Server using SSL (%s)\r\n", server.c_str());
#else
  DEBUG_SINRIC("[SinricPro:Websocket]: Connecting to WebSocket Server (%s)\r\n", server.c_str());
#endif

  if (_isConnected) {
    stop();
  }
  setExtraHeaders();
  webSocket.onEvent(std::bind(&WebsocketListener::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  webSocket.enableHeartbeat(WEBSOCKET_PING_INTERVAL, WEBSOCKET_PING_TIMEOUT, WEBSOCKET_RETRY_COUNT);
#ifdef WEBSOCKET_SSL
  webSocket.beginSSL(server.c_str(), SINRICPRO_SERVER_SSL_PORT, "/");
#else
  webSocket.begin(server.c_str(), SINRICPRO_SERVER_PORT, "/"); // server address, port and URL
#endif
}

void WebsocketListener::handle() {
  webSocket.loop();
}

void WebsocketListener::stop() {
  if (_isConnected) webSocket.disconnect();
  _begin = false;
}

bool WebsocketListener::isConnected() { 
  return _isConnected; 
}

void WebsocketListener::setRestoreDeviceStates(bool flag) { 
  this->restoreDeviceStates = flag; 
};


void WebsocketListener::sendMessage(String &message) {
  webSocket.sendTXT(message);
}

void WebsocketListener::onConnected(wsConnectedCallback callback) {
  _wsConnectedCb = callback; 
}

void WebsocketListener::onDisconnected(wsDisconnectedCallback callback) {
  _wsDisconnectedCb = callback; 
}

void WebsocketListener::onPong(std::function<void(uint32_t)> cb) {
webSocket.onPong(cb); 
}

void WebsocketListener::disconnect() {
  webSocket.disconnect(); 
}

void WebsocketListener::webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  (void) length;
  
  switch (type) {

    case WStype_DISCONNECTED:
      if (_isConnected) {
        DEBUG_SINRIC("[SinricPro:Websocket]: disconnected\r\n");
        if (_wsDisconnectedCb) _wsDisconnectedCb();
        _isConnected = false;
      }
      break;

    case WStype_CONNECTED:
      _isConnected = true;
      DEBUG_SINRIC("[SinricPro:Websocket]: connected\r\n");
      if (_wsConnectedCb) _wsConnectedCb();
      if (restoreDeviceStates) {
        restoreDeviceStates=false; 
        setExtraHeaders();
      }
      break;

    case WStype_TEXT: {
      SinricProMessage* request = new SinricProMessage(IF_WEBSOCKET, (char*)payload);
      DEBUG_SINRIC("[SinricPro:Websocket]: receiving data\r\n");
      receiveQueue->push(request);
      break;
    }

    default: break;
  }
}
}
#include "SinricPro.h"
#include "SinricProThermostat.h"

namespace SINRICPRO_NAMESPACE{

SinricProThermostat* SinricProClass::getDevice(String deviceId) {
  for (auto& device : devices) {
    if (deviceId == device->getDeviceId()) return device;
  }
  return nullptr;
}

SinricProThermostat& SinricProClass::getDeviceInstance(String deviceId) { 
  SinricProThermostat* tmp_device = getDevice(deviceId);
  if (tmp_device) return *tmp_device;
  
  DEBUG_SINRIC("[SinricPro]: Device \"%s\" does not exist. Creating new device\r\n", deviceId.c_str());
  SinricProThermostat& tmp_deviceInstance = add(deviceId);

  if (isConnected()) {
    DEBUG_SINRIC("[SinricPro]: Reconnecting to server.\r\n");
    reconnect();
  }

  return tmp_deviceInstance;
}

/**
 * @brief Initializing SinricProClass to be able to connect to SinricPro Server
 * 
 * @param appKey `String` containing APP_KEY (see credentials from https://sinric.pro )
 * @param appSecret `String` containing APP_SECRET (see credentials from https:://sinric.pro)
 * @param serverURL `String` containing SinricPro Server URL (default="ws.sinric.pro")
 * @section Example-Code
 * @code
 * #define APP_KEY           "YOUR-APP-KEY"      // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
 * #define APP_SECRET        "YOUR-APP-SECRET"   // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"
 * 
 * void setup() {
 *   SinricPro.begin(APP_KEY, APP_SECRET);
 * }
 * @endcode
 **/
void SinricProClass::begin(String appKey, String appSecret, String serverURL) {
  bool success = true;
  if (!appKey.length()) {
    DEBUG_SINRIC("[SinricPro:begin()]: App-Key \"%s\" is invalid!! Please check your app-key!! SinricPro will not work!\r\n", appKey.c_str());
    success = false;
  }
  if (!appSecret.length()) {
    DEBUG_SINRIC("[SinricPro:begin()]: App-Secret \"%s\" is invalid!! Please check your app-secret!! SinricPro will not work!\r\n", appSecret.c_str());
    success = false;
  }

  if(!success) {
    _begin = false;
    return;
  }

  this->appKey = appKey;
  this->appSecret = appSecret;
  this->serverURL = serverURL;
  _begin = true;
  _udpListener.begin(&receiveQueue);
}

SinricProThermostat& SinricProClass::add(String deviceId) {

  SinricProThermostat* newDevice = new SinricProThermostat(deviceId);
  DEBUG_SINRIC("[SinricPro:add()]: Adding device with id \"%s\".\r\n", deviceId.c_str());
  newDevice->begin(this);

  devices.push_back(newDevice);
  return *newDevice;
}

__attribute__ ((deprecated("Please use DeviceType& myDevice = SinricPro.add<DeviceType>(String);")))
void SinricProClass::add(SinricProThermostat* newDevice) {
  newDevice->begin(this);
  devices.push_back(newDevice);
}

__attribute__ ((deprecated("Please use DeviceType& myDevice = SinricPro.add<DeviceType>(String);")))
void SinricProClass::add(SinricProThermostat& newDevice) {
  newDevice.begin(this);
  devices.push_back(&newDevice);
}

/**
 * @brief Handles communication between device and SinricPro Server
 * 
 * This is the absolute main function which handles communication between your device and SinricPro Server. \n
 * It is responsible for connect, disconnect to SinricPro Server, handling requests, responses and events. \n
 * This function has to be called as often as possible. So it must be called in your main loop() function! \n
 * 
 * For proper function, begin() must be called with valid values for 'APP_KEY' and 'APP_SECRET' \n
 * @section handle Example-Code
 * @code
 * void loop() {
 *   SinricPro.handle();
 * }
 * @endcode
 **/
void SinricProClass::handle() {
  static bool begin_error = false;
  if (!_begin) {
    if (!begin_error) { // print this only once!
      DEBUG_SINRIC("[SinricPro:handle()]: ERROR! SinricPro.begin() failed or was not called prior to event handler\r\n");
      DEBUG_SINRIC("[SinricPro:handle()]:    -Reasons include an invalid app-key, invalid app-secret or no valid deviceIds)\r\n");
      DEBUG_SINRIC("[SinricPro:handle()]:    -SinricPro is disabled! Check earlier log messages for details.\r\n");
      begin_error = true;
    }
    return;
  }


  if (!isConnected()) connect();
  _websocketListener.handle();
  _udpListener.handle();

  handleReceiveQueue();
  handleSendQueue();
}

DynamicJsonDocument SinricProClass::prepareRequest(String deviceId, const char* action) {
  DynamicJsonDocument requestMessage(1024);
  JsonObject header = requestMessage.createNestedObject(FSTR_SINRICPRO_header);
  header[FSTR_SINRICPRO_payloadVersion] = 2;
  header[FSTR_SINRICPRO_signatureVersion] = 1;

  JsonObject payload = requestMessage.createNestedObject(FSTR_SINRICPRO_payload);
  payload[FSTR_SINRICPRO_action] = action;
  payload[FSTR_SINRICPRO_createdAt] = 0;
  payload[FSTR_SINRICPRO_deviceId] = deviceId;
  payload[FSTR_SINRICPRO_replyToken] = MessageID().getID();
  payload[FSTR_SINRICPRO_type] = FSTR_SINRICPRO_request;
  payload.createNestedObject(FSTR_SINRICPRO_value);
  return requestMessage;
}

void SinricProClass::handleResponse(DynamicJsonDocument& responseMessage) {
  (void) responseMessage;
  DEBUG_SINRIC("[SinricPro.handleResponse()]:\r\n");

  #ifndef NODEBUG_SINRIC
          serializeJsonPretty(responseMessage, DEBUG_ESP_PORT);
          Serial.println();
  #endif
}

void SinricProClass::handleRequest(DynamicJsonDocument& requestMessage, interface_t Interface) {
  DEBUG_SINRIC("[SinricPro.handleRequest()]: handling request\r\n");
  #ifndef NODEBUG_SINRIC
          serializeJsonPretty(requestMessage, DEBUG_ESP_PORT);
  #endif

  DynamicJsonDocument responseMessage = prepareResponse(requestMessage);

  // handle devices
  bool success = false;
  const char* deviceId = requestMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_deviceId];
  String action = requestMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_action] | "";
  String instance = requestMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_instanceId] | "";
  JsonObject request_value = requestMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_value];
  JsonObject response_value = responseMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_value];

  for (auto& device : devices) {
    if (device->getDeviceId() == deviceId && success == false) {
      SinricProRequest request {
        action,
        instance,
        request_value,
        response_value
      };
      success = device->handleRequest(request);
      responseMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_success] = success;
      if (!success) {
        if (responseMessageStr.length() > 0){
          responseMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_message] = responseMessageStr;
          responseMessageStr = ""; 
        } else {
          responseMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_message] = "Device did not handle \""+ action + "\"";
        }
      }
    }
  }

  String responseString;
  serializeJson(responseMessage, responseString);
  sendQueue.push(new SinricProMessage(Interface, responseString.c_str()));
}

void SinricProClass::handleReceiveQueue() {
  if (receiveQueue.size() == 0) return;

  DEBUG_SINRIC("[SinricPro.handleReceiveQueue()]: %i message(s) in receiveQueue\r\n", receiveQueue.size());
  while (receiveQueue.size() > 0) {
    SinricProMessage* rawMessage = receiveQueue.front();
    receiveQueue.pop();
    DynamicJsonDocument jsonMessage(1024);
    deserializeJson(jsonMessage, rawMessage->getMessage());

    bool sigMatch = false;

    if (strncmp(rawMessage->getMessage(), "{\"timestamp\":", 13) == 0 && strlen(rawMessage->getMessage()) <= 26) {
      sigMatch=true; // timestamp message has no signature...ignore sigMatch for this!
    } else {
      String signature = jsonMessage[FSTR_SINRICPRO_signature][FSTR_SINRICPRO_HMAC] | "";
      String payload = extractPayload(rawMessage->getMessage());
      String calculatedSignature = calculateSignature(appSecret.c_str(), payload);
      sigMatch = (calculatedSignature == signature);
    }

    String messageType = jsonMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_type];

    if (sigMatch) { // signature is valid process message
      DEBUG_SINRIC("[SinricPro.handleReceiveQueue()]: Signature is valid. Processing message...\r\n");
      extractTimestamp(jsonMessage);
      if (messageType == FSTR_SINRICPRO_response) handleResponse(jsonMessage);
      if (messageType == FSTR_SINRICPRO_request) handleRequest(jsonMessage, rawMessage->getInterface());
    } else {
      DEBUG_SINRIC("[SinricPro.handleReceiveQueue()]: Signature is invalid! Sending messsage to [dev/null] ;)\r\n");
    }
    delete rawMessage;
  }
}

void SinricProClass::handleSendQueue() {
  if (!isConnected()) return;
  if (!baseTimestamp) return;
  while (sendQueue.size() > 0) {
    DEBUG_SINRIC("[SinricPro:handleSendQueue()]: %i message(s) in sendQueue\r\n", sendQueue.size());
    DEBUG_SINRIC("[SinricPro:handleSendQueue()]: Sending message...\r\n");

    SinricProMessage* rawMessage = sendQueue.front(); sendQueue.pop();

    DynamicJsonDocument jsonMessage(1024);
    deserializeJson(jsonMessage, rawMessage->getMessage());
    jsonMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_createdAt] = getTimestamp();
    signMessage(appSecret, jsonMessage);

    String messageStr;

    serializeJson(jsonMessage, messageStr);
    #ifndef NODEBUG_SINRIC
            serializeJsonPretty(jsonMessage, DEBUG_ESP_PORT);
            Serial.println();
    #endif

    switch (rawMessage->getInterface()) {
      case IF_WEBSOCKET: DEBUG_SINRIC("[SinricPro:handleSendQueue]: Sending to websocket\r\n"); _websocketListener.sendMessage(messageStr); break;
      case IF_UDP:       DEBUG_SINRIC("[SinricPro:handleSendQueue]: Sending to UDP\r\n");_udpListener.sendMessage(messageStr); break;
      default:           break;
    }
    delete rawMessage;
    DEBUG_SINRIC("[SinricPro:handleSendQueue()]: message sent.\r\n");
  }
}

void SinricProClass::connect() {
  String deviceList;
  int i = 0;
  for (auto& device : devices) {
    String deviceId = device->getDeviceId();
    if (i>0) deviceList += ';';
    deviceList += device->getDeviceId();
    i++;
  }

  _websocketListener.begin(serverURL, appKey, deviceList, &receiveQueue);
}


void SinricProClass::stop() {
  _begin = false;
  DEBUG_SINRIC("[SinricPro:stop()\r\n");
  _websocketListener.stop();
}

bool SinricProClass::isConnected() {
  return _websocketListener.isConnected();
};

/**
 * @brief Set callback function for websocket connected event
 * 
 * @param cb Function pointer to a `ConnectedCallbackHandler` function
 * @return void
 * @see ConnectedCallbackHandler
 * @section onConnected Example-Code
 * @snippet callbacks.cpp onConnected
 **/
void SinricProClass::onConnected(ConnectedCallbackHandler cb) {
  _websocketListener.onConnected(cb);
}

/**
 * @brief Set callback function for websocket disconnected event
 * 
 * @param cb Function pointer to a `DisconnectedCallbackHandler` function
 * @return void
 * @see DisconnectedCallbackHandler
 * @section onDisconnected Example-Code
 * @snippet callbacks.cpp onDisconnected
 **/
void SinricProClass::onDisconnected(DisconnectedCallbackHandler cb) {
  _websocketListener.onDisconnected(cb);
}

void SinricProClass::onPong(std::function<void(uint32_t)> cb) { 
  _websocketListener.onPong(cb); 
}


void SinricProClass::reconnect() {
  DEBUG_SINRIC("SinricPro:reconnect(): disconnecting\r\n");
  stop();
  DEBUG_SINRIC("SinricPro:reconnect(): connecting\r\n");
  connect();
}

void SinricProClass::onConnect() { 
  DEBUG_SINRIC("[SinricPro]: Connected to \"%s\"!]\r\n", serverURL.c_str()); 
}

void SinricProClass::onDisconnect() { 
  DEBUG_SINRIC("[SinricPro]: Disconnect\r\n"); 
}


void SinricProClass::extractTimestamp(JsonDocument &message) {
  unsigned long tempTimestamp = 0;
  // extract timestamp from timestamp message right after websocket connection is established
  tempTimestamp = message["timestamp"] | 0;
  if (tempTimestamp) {
    baseTimestamp = tempTimestamp - (millis() / 1000);
    DEBUG_SINRIC("[SinricPro:extractTimestamp(): Got Timestamp %lu\r\n", tempTimestamp);
    return;
  }

  // extract timestamp from request message
  tempTimestamp = message[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_createdAt] | 0;
  if (tempTimestamp) {
    DEBUG_SINRIC("[SinricPro:extractTimestamp(): Got Timestamp %lu\r\n", tempTimestamp);
    baseTimestamp = tempTimestamp - (millis() / 1000);
    return;
  }
}


void SinricProClass::sendMessage(JsonDocument& jsonMessage) {
  if (!isConnected()) {
    DEBUG_SINRIC("[SinricPro:sendMessage()]: device is offline, message has been dropped\r\n");
    return;
  }
  DEBUG_SINRIC("[SinricPro:sendMessage()]: pushing message into sendQueue\r\n");
  String messageString;
  serializeJson(jsonMessage, messageString);
  sendQueue.push(new SinricProMessage(IF_WEBSOCKET, messageString.c_str()));
}

/**
 * @brief Enable / disable restore device states function
 * 
 * If this flag is enabled (`true`), SinricProServer will send last known device states to your device directly after connection to SinricPro server has been established. \n 
 * For every state the corresponding callback (like `onPowerState`) will be called \n
 * This is useful after a power failure / reboot of your device.
 * 
 * @param flag `true` = enabled \n `false`= disabled
 **/
void SinricProClass::restoreDeviceStates(bool flag) { 
  _websocketListener.setRestoreDeviceStates(flag);
}

/**
 * @brief operator[] is used tor create a new device instance or get an existing device instance
 * 
 * If the device is unknown to SinricProClass it will create a new device instance
 * @param deviceId a String containing deviceId for device that have to been created or retreived
 * @return returns a proxy object representing the reference to a device derrivered from SinricProDevice
 * @section Syntax
 * `<DeviceType> &reference = SinricPro[<DEVICE_ID>];`
 * @section operator[] Example-Code
 * @code
 * #define SWITCH_ID         "YOUR-DEVICE-ID"    // Should look like "5dc1564130xxxxxxxxxxxxxx"
 * ..
 *   SinricProSwitch &mySwitch = SinricPro[SWITCH_ID];
 * ..
 * @endcode
 **/ 
SinricProClass::proxy SinricProClass::operator[](const String deviceId) { 
  return proxy(this, deviceId); 
}

void SinricProClass::setResponseMessage(String &&message) { 
  responseMessageStr = message; 
}

/**
 * @brief Get the current timestamp
 * 
 * @return unsigned long current timestamp (unix epoch time)
 */
unsigned long SinricProClass::getTimestamp() { 
  return baseTimestamp + (millis()/1000); 
}

DynamicJsonDocument SinricProClass::prepareResponse(JsonDocument& requestMessage) {
  DynamicJsonDocument responseMessage(1024);
  JsonObject header = responseMessage.createNestedObject(FSTR_SINRICPRO_header);
  header[FSTR_SINRICPRO_payloadVersion] = 2;
  header[FSTR_SINRICPRO_signatureVersion] = 1;

  JsonObject payload = responseMessage.createNestedObject(FSTR_SINRICPRO_payload);
  payload[FSTR_SINRICPRO_action] = requestMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_action];
  payload[FSTR_SINRICPRO_clientId] = requestMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_clientId];
  payload[FSTR_SINRICPRO_createdAt] = 0;
  payload[FSTR_SINRICPRO_deviceId] = requestMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_deviceId];
  if (requestMessage[FSTR_SINRICPRO_payload].containsKey(FSTR_SINRICPRO_instanceId)) payload[FSTR_SINRICPRO_instanceId] = requestMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_instanceId];
  payload[FSTR_SINRICPRO_message] = FSTR_SINRICPRO_OK;
  payload[FSTR_SINRICPRO_replyToken] = requestMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_replyToken];
  payload[FSTR_SINRICPRO_success] = false;
  payload[FSTR_SINRICPRO_type] = FSTR_SINRICPRO_response;
  payload.createNestedObject(FSTR_SINRICPRO_value);
  return responseMessage;
}


DynamicJsonDocument SinricProClass::prepareEvent(String deviceId, const char* action, const char* cause) {
  DynamicJsonDocument eventMessage(1024);
  JsonObject header = eventMessage.createNestedObject(FSTR_SINRICPRO_header);
  header[FSTR_SINRICPRO_payloadVersion] = 2;
  header[FSTR_SINRICPRO_signatureVersion] = 1;

  JsonObject payload = eventMessage.createNestedObject(FSTR_SINRICPRO_payload);
  payload[FSTR_SINRICPRO_action] = action;
  payload[FSTR_SINRICPRO_cause].createNestedObject(FSTR_SINRICPRO_type);
  payload[FSTR_SINRICPRO_cause][FSTR_SINRICPRO_type] = cause;
  payload[FSTR_SINRICPRO_createdAt] = 0;
  payload[FSTR_SINRICPRO_deviceId] = deviceId;
  payload[FSTR_SINRICPRO_replyToken] = MessageID().getID();
  payload[FSTR_SINRICPRO_type] = FSTR_SINRICPRO_event;
  payload.createNestedObject(FSTR_SINRICPRO_value);
  return eventMessage;
}

}

using namespace SINRICPRO_NAMESPACE;

SinricProClass SinricPro;
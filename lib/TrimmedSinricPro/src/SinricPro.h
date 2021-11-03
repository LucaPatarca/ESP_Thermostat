/*
 *  Copyright (c) 2019 Sinric. All rights reserved.
 *  Licensed under Creative Commons Attribution-Share Alike (CC BY-SA)
 *
 *  This file is part of the Sinric Pro (https://github.com/sinricpro/)
 */

#pragma once

#include "SinricProInterface.h"
#include "SinricProDeviceInterface.h"
#include "SinricProWebsocket.h"
#include "SinricProUDP.h"
#include "SinricProSignature.h"
#include "SinricProMessageid.h"
#include "SinricProQueue.h"
#include "SinricProStrings.h"
#include "SinricProThermostat.h"

#include "SinricProNamespace.h"
namespace SINRICPRO_NAMESPACE {

/**
 * @brief Callback definition for onConnected function
 * 
 * Gets called when device is connected to SinricPro server
 * @param void
 * @return void
 */
using ConnectedCallbackHandler = std::function<void(void)>;

/**
 * @brief Callback definition for onDisconnected function
 * 
 * Gets called when device is disconnected from SinricPro server
 * @param void
 * @return void
 */
using DisconnectedCallbackHandler = std::function<void(void)>;

/**
 * @class SinricProClass
 * @ingroup SinricPro
 * @brief The main class of this library, handling communication between SinricPro Server and your devices
 **/
class SinricProClass : public SinricProInterface {
  friend class SinricProDevice;

  public:
    void               begin(String appKey, String appSecret, String serverURL = SINRICPRO_SERVER_URL);
    void               handle();
    void               stop();
    bool               isConnected();
    void               onConnected(ConnectedCallbackHandler cb);
    void               onDisconnected(DisconnectedCallbackHandler cb);
    void               onPong(std::function<void(uint32_t)> cb);
    void               restoreDeviceStates(bool flag);
    void               setResponseMessage(String &&message);
    unsigned long      getTimestamp() override;

    struct proxy {
      proxy(SinricProClass* ptr, String deviceId) : ptr(ptr), deviceId(deviceId) {}
      SinricProClass* ptr;
      String deviceId;
      operator SinricProThermostat&() { return as(); }
      SinricProThermostat& as() { return ptr->getDeviceInstance(deviceId); }
    };

    proxy               operator[](const String deviceId);
  protected:
    SinricProThermostat&         add(String deviceId);

    void                add(SinricProThermostat &newDevice);
    void                add(SinricProThermostat *newDevice);

    DynamicJsonDocument prepareResponse(JsonDocument &requestMessage);
    DynamicJsonDocument prepareEvent(String deviceId, const char *action, const char *cause) override;
    void                sendMessage(JsonDocument &jsonMessage) override;

  private:
    void                handleReceiveQueue();
    void                handleSendQueue();

    void                handleRequest(DynamicJsonDocument& requestMessage, interface_t Interface);
    void                handleResponse(DynamicJsonDocument& responseMessage);

    DynamicJsonDocument prepareRequest(String deviceId, const char* action);

    void                connect();
    void                disconnect();
    void                reconnect();

    void                onConnect();
    void                onDisconnect();

    void                extractTimestamp(JsonDocument &message);

    SinricProThermostat* getDevice(String deviceId);

    SinricProThermostat&         getDeviceInstance(String deviceId);

    std::vector<SinricProThermostat*> devices;

    String              appKey;
    String              appSecret;
    String              serverURL;

    WebsocketListener   _websocketListener;
    UdpListener         _udpListener;
    SinricProQueue_t    receiveQueue;
    SinricProQueue_t    sendQueue;

    unsigned long       baseTimestamp = 0;

    bool                _begin = false;
    String              responseMessageStr = "";
};

} //namespace

using namespace SINRICPRO_NAMESPACE;

extern SinricProClass SinricPro;

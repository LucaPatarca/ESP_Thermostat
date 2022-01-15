/*
 *  Copyright (c) 2019 Sinric. All rights reserved.
 *  Licensed under Creative Commons Attribution-Share Alike (CC BY-SA)
 *
 *  This file is part of the Sinric Pro (https://github.com/sinricpro/)
 */

#pragma once

#include <WebSocketsClient.h>
#include "SinricProQueue.h"

#include "SinricProNamespace.h"
namespace SINRICPRO_NAMESPACE {

#if !defined(WEBSOCKETS_VERSION_INT) || (WEBSOCKETS_VERSION_INT < 2003005)
#error "Wrong WebSockets Version! Minimum Version is 2.3.5!!!"
#endif


class AdvWebSocketsClient : public WebSocketsClient {
  public:
    AdvWebSocketsClient();
    void onPong(std::function<void(uint32_t)> cb);
  protected:
    void messageReceived(WSclient_t * client, WSopcode_t opcode, uint8_t * payload, size_t length, bool fin);
  private:
    std::function<void(uint32_t)> _rttCb;
};

using wsConnectedCallback = std::function<void(void)>;
using wsDisconnectedCallback = std::function<void(void)>;

class WebsocketListener {
  public:

    WebsocketListener();
    ~WebsocketListener();

    void                    begin(String server, String appKey, String deviceIds, SinricProQueue_t* receiveQueue);
    void                    handle();
    void                    stop();
    bool                    isConnected();
    void                    setRestoreDeviceStates(bool flag);

    void                    sendMessage(String &message);

    void                    onConnected(wsConnectedCallback callback);
    void                    onDisconnected(wsDisconnectedCallback callback);
    void                    onPong(std::function<void(uint32_t)> cb);

    void                    disconnect();
  private:
    bool                    _begin;
    bool                    _isConnected;
    bool                    restoreDeviceStates;

    AdvWebSocketsClient     webSocket;
    wsConnectedCallback     _wsConnectedCb;
    wsDisconnectedCallback  _wsDisconnectedCb;

    void                    webSocketEvent(WStype_t type, uint8_t * payload, size_t length);
    void                    setExtraHeaders();
    SinricProQueue_t*       receiveQueue;
    String                  deviceIds;
    String                  appKey;
};

} // SINRICPRO_NAMESPACE
#include <wifi.h>
#include <credentials.h>

WifiController::WifiController()
{
    wifiConnectHandler = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP &event)
                                                 { this->onWiFiConnect(event); });
    wifiDisconnectHandler = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected &event)
                                                           { this->onWiFiDisconnect(event); });
}

void WifiController::notifiStatus(WiFiStatus status)
{
    State::Instance().setwWifiStatus(status);
}

void WifiController::onWiFiConnect(const WiFiEventStationModeGotIP &event)
{
#ifdef WIFI_DEBUG
    Serial.printf("\r\n[Wifi]: Connected\n");
#endif
    notifiStatus(WiFiStatus::CONNECTED);
    _firstConnection = true;
}

void WifiController::onWiFiDisconnect(const WiFiEventStationModeDisconnected &event)
{
#ifdef WIFI_DEBUG
    Serial.printf("\r\n[Wifi]: Disconnected for reason: %d\n", event.reason);
#endif
    if(event.reason == WiFiDisconnectReason::WIFI_DISCONNECT_REASON_AUTH_LEAVE){
        notifiStatus(WiFiStatus::DISCONNECTED);
    }
    if(event.reason == WiFiDisconnectReason::WIFI_DISCONNECT_REASON_ASSOC_LEAVE){
        return;
    }
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    if(!_firstConnection){
        notifiStatus(WiFiStatus::CONNECTING);
    }
}

void WifiController::connect()
{
#ifdef WIFI_DEBUG
    Serial.printf("\r\n[Wifi]: Connecting\n");
#endif
    notifiStatus(WiFiStatus::CONNECTING);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
}

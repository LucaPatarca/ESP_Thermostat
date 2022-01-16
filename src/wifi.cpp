#include <wifi.h>
#include <credentials.h>
#include <sdebug.h>

//TODO add emergency mode that should be used to set things like the time and the program (when there is no internet)
// evaluate if it's better to implement with udp broadcast using router or by making the thermostat a router (less elengant but safer)

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
    INFO("connected");
    notifiStatus(WiFiStatus::CONNECTED);
    _firstConnection = true;
}

void WifiController::onWiFiDisconnect(const WiFiEventStationModeDisconnected &event)
{
    FWARN("disconnected for reason: %d\n", event.reason);
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
    INFO("connecting");
    notifiStatus(WiFiStatus::CONNECTING);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
}

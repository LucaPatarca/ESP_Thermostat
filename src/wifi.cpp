#include <wifi.h>
#include <credentials.h>
#include <ntime.h>

WifiController::WifiController()
{
    wifiConnectHandler = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP &event)
                                                 { this->onWiFiConnect(event); });
    wifiDisconnectHandler = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected &event)
                                                           { this->onWiFiDisconnect(event); });
    WiFi.onStationModeDHCPTimeout([this]()
                                  {
        Serial.printf("[Wifi] DHCP Timeout");
        connect(); });
}

void WifiController::notifiStatus(WiFiStatus status)
{
    for (WiFiListener *listener : _listeners)
    {
        listener->onWiFiStatus(status);
    }
}

void WifiController::onWiFiConnect(const WiFiEventStationModeGotIP &event)
{
#ifdef WIFI_DEBUG
    Serial.printf("\r\n[Wifi]: Connected\n");
#endif
    notifiStatus(WiFiStatus::CONNECTED);
}

void WifiController::onWiFiDisconnect(const WiFiEventStationModeDisconnected &event)
{
#ifdef WIFI_DEBUG
    Serial.printf("\r\n[Wifi]: Disconnected\n");
#endif
    notifiStatus(WiFiStatus::DISCONNECTED);
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASS);
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

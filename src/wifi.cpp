#include <wifi.h>
#include <credentials.h>

void WifiController::notifiStatus(WiFiStatus status)
{
    for (WiFiListener *listener : _listeners)
    {
        listener->onWiFiStatus(status);
    }
}

void WifiController::onWiFiConnect(const WiFiEventStationModeGotIP &event)
{
}

void WifiController::onWiFiDisconnect(const WiFiEventStationModeDisconnected &event)
{
}

void WifiController::connect()
{
    Serial.printf("\r\n[Wifi]: Connecting");

    notifiStatus(WiFiStatus::CONNECTING);

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.printf(".");
        delay(250);
    }
    //TODO implementare listener
    IPAddress localIP = WiFi.localIP();
    Serial.printf("connected!\r\n[WiFi]: IP-Address is %d.%d.%d.%d\r\n", localIP[0], localIP[1], localIP[2], localIP[3]);

    notifiStatus(WiFiStatus::CONNECTED);
}

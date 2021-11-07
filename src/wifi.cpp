#include <wifi.h>
#include <credentials.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

void WifiController::notifiStatus(WiFiStatus status){
    for (WiFiListener *listener : _listeners)
    {
        listener->onWiFiStatus(status);
    }
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
    IPAddress localIP = WiFi.localIP();
    Serial.printf("connected!\r\n[WiFi]: IP-Address is %d.%d.%d.%d\r\n", localIP[0], localIP[1], localIP[2], localIP[3]);

    notifiStatus(WiFiStatus::CONNECTED);
}

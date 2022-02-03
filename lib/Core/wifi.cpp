#include <wifi.h>
#include <credentials.h>
#include <sdebug.h>

#define CHECK_INTERVAL 15000 //in milliseconds

//TODO add emergency mode that should be used to set things like the time and the program (when there is no internet)
// evaluate if it's better to implement with udp broadcast using router or by making the thermostat a router (less elengant but safer)

WifiController::WifiController()
{
    m_wifiConnectHandler = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP &event)
                                                   { this->onWiFiConnect(event); });
    m_wifiDisconnectHandler = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected &event)
                                                             { this->onWiFiDisconnect(event); });
    memset(&m_options, 0, sizeof(struct ping_option));
    m_options.count = 1;
    m_options.coarse_time = 1;
    m_options.ip = IPAddress(8, 8, 8, 8);
    m_options.recv_function = reinterpret_cast<ping_recv_function>(&WifiController::onPingReceive);

    m_server.on("/set-wifi", HTTP_POST, [this]()
                { onRecvWifiCredentials(); });
    m_server.on("/set-api", HTTP_POST, [this]()
                { onRecvApiCredentials(); });
}

void WifiController::notifiStatus(WiFiStatus status)
{
    State::Instance().setwWifiStatus(status);
}

void WifiController::onWiFiConnect(const WiFiEventStationModeGotIP &event)
{
    INFO("connected");
    INFO("IP: %d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    notifiStatus(WiFiStatus::CONNECTED);
    m_firstConnection = true;
}

void WifiController::onWiFiDisconnect(const WiFiEventStationModeDisconnected &event)
{
    WARN("disconnected for reason: %d\n", event.reason);
    if (event.reason == WiFiDisconnectReason::WIFI_DISCONNECT_REASON_AUTH_LEAVE)
    {
        notifiStatus(WiFiStatus::DISCONNECTED);
    }
    if (event.reason == WiFiDisconnectReason::WIFI_DISCONNECT_REASON_ASSOC_LEAVE)
    {
        return;
    }
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    if (!m_firstConnection)
    {
        notifiStatus(WiFiStatus::CONNECTING);
    }
}

//static function
void WifiController::onPingReceive(void *opt, void *resp)
{
    ping_resp *ping_resp = reinterpret_cast<struct ping_resp *>(resp);
    WifiController &controller = WifiController::Instance();
    if (ping_resp->ping_err == -1)
    {
        INFO("ping error");
        if (State::Instance().getWifiStatus() == WiFiStatus::CONNECTED)
        {
            controller.notifiStatus(WiFiStatus::CONNECTED_NO_INTERNET);
            controller.m_server.begin();
        }
    }
    else
    {
        INFO("ping success");
        controller.notifiStatus(WiFiStatus::CONNECTED);
    }
}

void WifiController::onRecvWifiCredentials()
{
    if (!m_server.hasArg("SSID") || !m_server.hasArg("PASS"))
    {
        WARN("Invalid Request");
        m_server.send(400, "text/plain", "Invalid Request");
    }
    else
    {
        INFO("received wifi credentials");
        String ssid = m_server.arg("SSID");
        String pass = m_server.arg("PASS");
        State::Instance().setWifiCredentials(ssid.c_str(), pass.c_str());
        m_server.send(200, "text/plain", "Ok");
        delay(1000);
        ESP.reset();
    }
}

void WifiController::onRecvApiCredentials()
{
    if (!m_server.hasArg("KEY") || !m_server.hasArg("SECRET") || !m_server.hasArg("DEVICEID"))
    {
        WARN("Invalid Request");
        m_server.send(400, "text/plain", "Invalid Request");
    }
    else
    {
        INFO("received api credentials");
        String key = m_server.arg("KEY");
        String secret = m_server.arg("SECRET");
        String deviceID = m_server.arg("DEVICEID");
        State::Instance().setApiCredentials(key.c_str(), secret.c_str(), deviceID.c_str());
        m_server.send(200, "text/plain", "Ok");
        delay(1000);
        ESP.reset();
    }
}

void WifiController::connect()
{
    Config_t &config = State::Instance().getConfig();
    if (config.wifiSet)
    {
        INFO("connecting");
        notifiStatus(WiFiStatus::CONNECTING);
        WiFi.mode(WIFI_STA);
        WiFi.begin(config.wifiSSID, config.wifiPASS);
    }
    else
    {
        INFO("configuration mode");
        WiFi.softAP("ESP_Thermostat_config");
    }
    if (!config.wifiSet || !config.apiSet)
    {
        m_server.begin();
    }
}

void WifiController::handle()
{
    if (millis() > m_lastCheck + CHECK_INTERVAL)
    {
        ping_start(&m_options);
        m_lastCheck = millis();
    }
    m_server.handleClient();
}
#include <wifi.h>
#include <sdebug.h>

#define CHECK_INTERVAL 15000 // in milliseconds

#define WIFI_SSID_ARG_NAME "SSID"
#define WIFI_PASS_ARG_NAME "PASS"
#define MQTT_SERVER_ARG_NAME "server"
#define MQTT_PORT_ARG_NAME "port"
#define MQTT_USER_ARG_NAME "user"
#define MQTT_PASS_ARG_NAME "password"

// TODO add emergency mode that should be used to set things like the time and the program (when there is no internet)
//  evaluate if it's better to implement with udp broadcast using router or by making the thermostat a router (less elengant but safer)

WifiController::WifiController()
{
    // m_wifiConnectHandler = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP &event)
    //                                                { this->onWiFiConnect(event); });
    m_wifiDisconnectHandler = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected &event)
                                                             { this->onWiFiDisconnect(event); });
    memset(&m_options, 0, sizeof(struct ping_option));
    m_options.count = 1;
    m_options.coarse_time = 1;
    m_options.ip = IPAddress(8, 8, 8, 8);
    m_options.recv_function = reinterpret_cast<ping_recv_function>(&WifiController::onPingReceive);

    m_server.on("/set-wifi", HTTP_POST, [this]()
                { onRecvWifiCredentials(); });
    m_server.on("/set-mqtt", HTTP_POST, [this]()
                { onRecvMQTTCredentials(); });
    m_connected = false;
}

void WifiController::notifiStatus(WiFiStatus status)
{
    State::Instance().setWifiStatus(status);
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
    WiFi.begin(State::Instance().getConfig().wifiSSID, State::Instance().getConfig().wifiPASS);
    if (!m_firstConnection)
    {
        notifiStatus(WiFiStatus::CONNECTING);
    }
}

// static function
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
    if (!m_server.hasArg(WIFI_SSID_ARG_NAME) || !m_server.hasArg("PASS"))
    {
        WARN("Invalid Request");
        m_server.send(400, "text/plain", "Invalid Request");
        return;
    }
    INFO("received wifi credentials");
    String ssid = m_server.arg(WIFI_SSID_ARG_NAME);
    String pass = m_server.arg("PASS");
    State::Instance().setWifiCredentials(ssid.c_str(), pass.c_str());
    m_token = generateToken();
    INFO("generated token: %s", (*m_token).c_str());
    m_server.sendHeader("AuthorizationToken", *m_token);
    m_server.send(200, "text/plain", WiFi.macAddress().c_str());
    delay(1000);
    WiFi.disconnect();
    connect();
}

void WifiController::onRecvMQTTCredentials()
{
    notifiStatus(WiFiStatus::CONNECTING);
    if (!m_server.hasHeader("Authorization") || m_token == nullptr)
    {
        WARN("No authorization");
        m_server.send(401, "text/plain", "Unauthorized");
        return;
    }
    if (!m_server.hasArg(MQTT_SERVER_ARG_NAME) || 
        !m_server.hasArg(MQTT_PORT_ARG_NAME) || 
        !m_server.hasArg(MQTT_USER_ARG_NAME) || 
        !m_server.hasArg(MQTT_PASS_ARG_NAME))
    {
        WARN("Invalid Request");
        m_server.send(400, "text/plain", "Invalid Request");
        return;
    }
    String token;
    int i = 0;
    while (token.isEmpty() && i < m_server.headers())
    {
        if (m_server.headerName(i) == "Authorization")
        {
            token = m_server.header(i);
        }
        i++;
    }
    if (*m_token != token)
    {
        WARN("Wrong Token");
        m_server.send(403, "text/plain", "Forbidden");
        return;
    }
    delete m_token;
    m_token = nullptr;
    INFO("received mqtt credentials");
    String server = m_server.arg(MQTT_SERVER_ARG_NAME);
    int port = m_server.arg(MQTT_PORT_ARG_NAME).toInt();
    String user = m_server.arg(MQTT_USER_ARG_NAME);
    String password = m_server.arg(MQTT_PASS_ARG_NAME);
    State::Instance().setMQTTCredentials(server.c_str(), user.c_str(), password.c_str(), port);
    m_server.send(200, "text/plain", "Ok");
    delay(1000);
    notifiStatus(WiFiStatus::CONNECTED);
}

String *WifiController::generateToken()
{
    uint32_t number = ESP.getChipId();
    number = number * millis();
    number = number * ESP.getVcc();
    char token[9];
    const char *letters = "0123456789ABCDEF";
    for (int i = 0; i < 8; i++)
    {
        uint8_t pos = number & 0b1111;
        token[i] = letters[pos];
        number = number >> 4;
    }
    token[8] = 0;
    return new String(token);
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
    if (!config.wifiSet || config.mqttSet)
    {
        m_server.begin();
        INFO("started http server");
    }
}

void WifiController::handle()
{
    // if (millis() > m_lastCheck + CHECK_INTERVAL)
    // {
    //     ping_start(&m_options);
    //     m_lastCheck = millis();
    // }
    if(WiFi.isConnected() != m_connected){
        m_connected = WiFi.isConnected();
        if(m_connected){
            onWiFiConnect(WiFiEventStationModeGotIP());
        }
    }
    m_server.handleClient();
}
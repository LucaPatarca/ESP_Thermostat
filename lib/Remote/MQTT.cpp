#include <MQTT.h>
#include <sdebug.h>

#define CURRENT_TEMP_GET_TOPIC "esp-thermostat/current-temperature/get"
#define TEMP_SETPOINT_GET_TOPIC "esp-thermostat/setpoint-temperature/get"
#define TEMP_SETPOINT_SET_TOPIC "esp-thermostat/setpoint-temperature/set"
#define MODE_GET_TOPIC "esp-thermostat/mode/get"
#define MODE_SET_TOPIC "esp-thermostat/mode/set"

MQTTController::MQTTController() : m_state(State::Instance())
{
    m_mqttClient.setClient(m_wifiClient);
}

String MQTTController::convertPayloadToStr(byte *payload, unsigned int length)
{
    char s[length + 1];
    s[length] = 0;
    for (int i = 0; i < (int)length; ++i)
        s[i] = payload[i];
    String tempRequestStr(s);
    return tempRequestStr;
}

void MQTTController::MQTTCallback(char *topic, byte *payload, unsigned int length)
{
    const String setpointSetTopic(TEMP_SETPOINT_SET_TOPIC);
    const String modeSetTopic(MODE_SET_TOPIC);

    const String topicStr(topic);
    String payloadStr = convertPayloadToStr(payload, length);

    if (topicStr == setpointSetTopic)
    {
        INFO("Received target temperature: %s", payloadStr.c_str());
        float newTemp = payloadStr.toFloat();
        m_state.setTargetTemperature(Cause::REMOTE, newTemp);
    }
    else if (topicStr == modeSetTopic)
    {
        INFO("Received mode: %s", payloadStr.c_str());
        if (payloadStr == "heat")
            m_state.setThermostatMode(Cause::REMOTE, Mode::ON);
        else if (payloadStr == "off")
            m_state.setThermostatMode(Cause::REMOTE, Mode::OFF);
        else{
            WARN("Received unknown mode");
        }
    }
}

void MQTTController::targetTemperatureChanged(Cause cause)
{
    sniprintf(m_msgBuf, MSG_BUFFER_SIZE, "%.1f", m_state.getTargetTemperature());
    if (m_mqttClient.publish(TEMP_SETPOINT_GET_TOPIC, m_msgBuf))
    {
        INFO("Send target temperature Ok");
    }
    else
    {
        ERROR("Error sending target temperature");
    }
}
void MQTTController::powerStateChanged(Cause cause)
{
    sniprintf(m_msgBuf, MSG_BUFFER_SIZE, "%s", m_state.getPowerState() ? "heat" : "off");
    if (m_mqttClient.publish(MODE_GET_TOPIC, m_msgBuf))
    {
        INFO("Send thermostat mode Ok");
    }
    else
    {
        ERROR("Error sending thermostat mode");
    }
}
void MQTTController::currentTemperatureChanged()
{
    sniprintf(m_msgBuf, MSG_BUFFER_SIZE, "%.1f", m_state.getCurrentTemperature().temp);
    if (m_mqttClient.publish(CURRENT_TEMP_GET_TOPIC, m_msgBuf))
    {
        INFO("Send current temperature Ok");
    }
    else
    {
        ERROR("Error sending current temperature");
    }
}
void MQTTController::thermostatModeChanged(Cause cause) {}

void MQTTController::connect()
{
    const char *mqttServer = m_state.getConfig().mqttServer;
    int mqttPort = m_state.getConfig().mqttPort;
    m_mqttClient.setServer(mqttServer, mqttPort);
    m_mqttClient.setCallback([this](char *topic, byte *payload, unsigned int length)
                             { this->MQTTCallback(topic, payload, length); });
    const char *mqttUser =  m_state.getConfig().mqttUser;
    const char *mqttPassword = m_state.getConfig().mqttPassword;
    INFO("Attempting MQTT connection (%s:%d user=%s pass=%s)", mqttServer,
         mqttPort, mqttUser, mqttPassword);
    const char *clientId = "esp-thermostat";
    if (m_mqttClient.connect(clientId, mqttUser, mqttPassword))
    {
        INFO("MQTT connected");
        m_mqttClient.subscribe(TEMP_SETPOINT_SET_TOPIC);
        m_mqttClient.subscribe(MODE_SET_TOPIC);
        currentTemperatureChanged();
        targetTemperatureChanged(Cause::AUTO);
        powerStateChanged(Cause::AUTO);
    }
    else
    {
        ERROR("Failed to connect to MQTT %d", m_mqttClient.state());
    }
}
void MQTTController::handle()
{
    if (!m_mqttClient.connected() && m_state.getConfig().mqttSet && m_state.getWifiStatus() == WiFiStatus::CONNECTED && millis() > m_lastConnectionAttempt + 5000)
    {
        connect();
        m_lastConnectionAttempt = millis();
    }
    m_mqttClient.loop();
}

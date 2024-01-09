#include <MQTT.h>
#include <sdebug.h>

#define CURRENT_TEMP_GET_TOPIC "esp-therm/curr-temp/get"
#define CURRENT_HUMIDITY_GET_TOPIC "esp-therm/curr-hum/get"
#define TEMP_SETPOINT_GET_TOPIC "esp-therm/temp/get"
#define TEMP_SETPOINT_SET_TOPIC "esp-therm/temp/set"
#define MODE_GET_TOPIC "esp-therm/mode/get"
#define MODE_SET_TOPIC "esp-therm/mode/set"
#define MQTT_CONFIG_TOPIC "homeassistant/climate/esp-thermostat/config"

const char MQTT_CONFIG_STRING [] = "{\"name\": \"Termosifoni\", \"uniq_id\": \"esp-therm1\", \"dev_cla\": \"climate\", \"dev\": {\"ids\": [\"esp-therm1\"], \"name\": \"Termosifoni\" }, \"modes\": [ \"off\", \"heat\" ], \"mode_cmd_t\": \"esp-therm/mode/set\", \"mode_stat_t\": \"esp-therm/mode/get\", \"temp_cmd_t\": \"esp-therm/temp/set\", \"temp_stat_t\": \"esp-therm/temp/get\", \"curr_temp_t\": \"esp-therm/curr-temp/get\", \"current_humidity_topic\": \"esp-therm/curr-hum/get\", \"precision\": 0.1}";

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
    if (!m_mqttClient.publish(TEMP_SETPOINT_GET_TOPIC, m_msgBuf))
    {
        ERROR("Error sending target temperature");
        return;
    }
    INFO("Send target temperature Ok");
}
void MQTTController::powerStateChanged(Cause cause)
{
    sniprintf(m_msgBuf, MSG_BUFFER_SIZE, "%s", m_state.getPowerState() ? "heat" : "off");
    if (!m_mqttClient.publish(MODE_GET_TOPIC, m_msgBuf))
    {
        ERROR("Error sending thermostat mode");
        return;
    }
    INFO("Send thermostat mode Ok");
}
void MQTTController::currentTemperatureChanged()
{
    auto temp = m_state.getCurrentTemperature();
    sniprintf(m_msgBuf, MSG_BUFFER_SIZE, "%.1f", temp.temp);
    if (!m_mqttClient.publish(CURRENT_TEMP_GET_TOPIC, m_msgBuf))
    {
        ERROR("Error sending current temperature");
        return;
    }
    sniprintf(m_msgBuf, MSG_BUFFER_SIZE, "%.1f", temp.humidity);
    if (!m_mqttClient.publish(CURRENT_HUMIDITY_GET_TOPIC, m_msgBuf))
    {
        ERROR("Error sending current humidity");
        return;
    }
    INFO("Send current temperature Ok");
}
void MQTTController::thermostatModeChanged(Cause cause) {}

void MQTTController::connect()
{
    const char *mqttServer = m_state.getConfig().mqttServer;
    int mqttPort = m_state.getConfig().mqttPort;
    m_mqttClient.setServer(mqttServer, mqttPort);
    m_mqttClient.setCallback([this](char *topic, byte *payload, unsigned int length)
                             { this->MQTTCallback(topic, payload, length); });
    if(!m_mqttClient.setBufferSize(1024)) {
        ERROR("Failed to set buffer size");
    }
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
        if(!m_mqttClient.publish(MQTT_CONFIG_TOPIC, MQTT_CONFIG_STRING)) {
            ERROR("Failed to send configuration, status: %d", m_mqttClient.state());
        }
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

#pragma once

#include <WiFiUdp.h>
#include <NTPClient.h>
#include <listeners.h>

enum Mode
{
    OFF,
    ON,
    PROGRAM
};

enum TemperatureTrend
{
    STABLE,
    DROP,
    RISE,
};

typedef struct Temperature
{
    float temp;
    float humidity;
    TemperatureTrend trend;
    float coefficient;

    bool operator ==(const Temperature &other) const {
        return !(temp - other.temp >= 0.1 || other.temp - temp >= 0.1) &&
            !(humidity - other.humidity >= 0.5 || other.humidity - humidity >= 0.5) &&
            trend == other.trend &&
            !(coefficient - other.coefficient >= 0.05 || other.coefficient - coefficient >= 0.05);
    }
} Temperature_t;

enum WiFiStatus
{
    DISCONNECTED,
    CONNECTED,
    CONNECTING,
    CONNECTED_NO_INTERNET,
};

typedef struct Time{
    int hour;
    int minutes;
    int day;
    bool operator !=(const Time &rhs) const {
        return this->hour != rhs.hour && this->minutes != rhs.minutes && this->day != rhs.day;
    }
} Time_t;

typedef struct {
    unsigned char temps[48];
} DayProgram_t;

typedef struct {
    DayProgram_t days[7];
} WeekProgram_t;

typedef struct {
    bool wifiSet;
    char wifiSSID[64];
    char wifiPASS[64];
    bool apiSet;
    char apiKey[64];
    char apiSecret[128];
    char apiDeviceID[32];
    WeekProgram_t program;
} Config_t;

class State
{
public:
    static State& Instance(){
        static State state;
        return state;
    }

    State(const State&) = delete;

    void setBoilerState(bool);
    void setTargetTemperature(Cause, float);
    void setPowerState(Cause, bool);
    void setThermostatMode(Cause, Mode);
    void setCurrentTemperature(const Temperature_t&);
    void setWifiStatus(WiFiStatus);
    bool setProgram(const WeekProgram_t&);
    void setWifiCredentials(const char *SSID, const char *pass);
    void setApiCredentials(const char *apiKey, const char *apiSecret, const char *apiDeviceID);

    bool getBoilerState() const;
    float getTargetTemperature() const;
    bool getPowerState() const;
    Mode getThermostatMode() const;
    const Temperature_t& getCurrentTemperature() const;
    WiFiStatus getWifiStatus() const;
    Time_t getTime();
    String getFormattedTime();
    Config_t& getConfig();

    void addListener(StateListener*);
    void reset();
    void factoryReset();

private:
    bool m_boilerState;
    float m_targetTemperature;
    bool m_powerState;
    Mode m_thermostatMode;
    Temperature_t m_currentTemperature;
    WiFiStatus m_wifiStatus;
    Config_t m_config;

    WiFiUDP m_udp;
    NTPClient m_client;

    StateListener *m_listener;

    void loadConfig();
    bool saveConfig();

    State();
};
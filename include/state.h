#pragma once

#include <listeners.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

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

typedef struct
{
    float temp;
    float humidity;
    TemperatureTrend trend;
    float coefficient;
} Temperature_t;

enum WiFiStatus
{
    DISCONNECTED,
    CONNECTED,
    CONNECTING,
};

typedef struct Time{
    int hour;
    int minutes;
    int day;
    bool operator !=(Time &rhs) const {
        return this->hour != rhs.hour && this->minutes != rhs.minutes && this->day != rhs.day;
    }
} Time_t;

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
    void setCurrentTemperature(Temperature_t&&);
    void setwWifiStatus(WiFiStatus);

    bool getBoilerState() const;
    float getTargetTemperature() const;
    bool getPowerState() const;
    Mode getThermostatMode() const;
    const Temperature_t& getCurrentTemperature() const;
    WiFiStatus getWifiStatus() const;

    Time_t getTime();
    String getFormattedTime();

    void addListener(StateListener*);

private:
    bool m_boilerState;
    float m_targetTemperature;
    bool m_powerState;
    Mode m_thermostatMode;
    Temperature_t m_currentTemperature;
    WiFiStatus m_wifiStatus;

    WiFiUDP m_udp;
    NTPClient m_client;

    StateListener *m_listener;

    State();
};
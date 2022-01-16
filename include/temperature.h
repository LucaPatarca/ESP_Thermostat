#pragma once

#include <DHT.h>
#include <state.h>

class TemperatureController
{
private:
    DHT m_sensor;
    TemperatureTrend m_lastTrend;
    float m_lastTemp;
    float m_smoothTemp;
    int m_stableCount;
    int m_riseCount;
    int m_dropCount;
    unsigned long m_updateTime;

    float smoothe(const float, const float);
    TemperatureTrend computeTrend();
    float computeCoefficient();

    TemperatureController();

public:
    TemperatureController(const TemperatureController&) = delete;

    static TemperatureController& Instance(){
        static TemperatureController controller;
        return controller;
    }

    void handle();
};
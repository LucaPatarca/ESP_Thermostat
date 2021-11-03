#pragma once

#include <DHT.h>

// #define TEMPERATURE_DEBUG

#ifdef ENABLE_DEBUG
#define TEMPERATURE_DEBUG
#endif

#define SMOOTH_FACTOR 4
#define SENSOR_PIN D5

enum TemperatureTrend{
    DROP,
    RISE,
    STABLE
};

struct Temperature{
    float temp;
    float humidity;
    TemperatureTrend trend;
    float coefficient;
};

class TemperatureController{
    private:
        DHT *m_sensor;
        TemperatureTrend m_lastTrend;
        float m_lastTemp;
        float m_smoothTemp;
        int m_stableCount;

        float smoothe(const float, const float);
    public:
        TemperatureController();

        Temperature getTemperature();
};
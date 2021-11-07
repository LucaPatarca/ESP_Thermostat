#pragma once

#include <DHT.h>
#include <EventEmitter.h>
#include <TemperatureListener.h>

// #define TEMPERATURE_DEBUG

#ifdef ENABLE_DEBUG
#define TEMPERATURE_DEBUG
#endif

#define SMOOTH_FACTOR 4
#define SENSOR_PIN D5

#define TEMP_EVENT_INTERVAL 60000 //in milliseconds

class TemperatureController : public EventEmitter<TemperatureListener>
{
private:
    DHT *_sensor;
    TemperatureTrend _lastTrend;
    float _lastTemp;
    float _smoothTemp;
    int _stableCount;
    unsigned long _updateTime;

    float smoothe(const float, const float);
    TemperatureTrend computeTrend();
    float computeCoefficient();

public:
    TemperatureController();

    void handle();
};
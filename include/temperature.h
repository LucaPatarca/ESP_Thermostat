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
#define STABLE_THRESHOLD 10
#define RISE_THRESHOLD 3
#define DROP_THRESHOLD 3
#define CHANGE_THRESHOLD 0.1

class TemperatureController : public EventEmitter<TemperatureListener>
{
private:
    DHT *_sensor;
    TemperatureTrend _lastTrend;
    float _lastTemp;
    float _smoothTemp;
    int _stableCount;
    int _riseCount;
    int _dropCount;
    unsigned long _updateTime;

    float smoothe(const float, const float);
    TemperatureTrend computeTrend();
    float computeCoefficient();

public:
    TemperatureController();

    void handle();
};
#pragma once

#include <DHT.h>
#include <state.h>

// #define TEMPERATURE_DEBUG

#define SMOOTH_FACTOR 4
#define SENSOR_PIN D5

#define TEMP_EVENT_INTERVAL 60000 //in milliseconds
#define STABLE_THRESHOLD 10
#define RISE_THRESHOLD 3
#define DROP_THRESHOLD 3
#define CHANGE_THRESHOLD 0.1

class TemperatureController
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

    TemperatureController();

public:
    TemperatureController(TemperatureController&) = delete;

    static TemperatureController& Instance(){
        static TemperatureController controller;
        return controller;
    }

    void handle();
};
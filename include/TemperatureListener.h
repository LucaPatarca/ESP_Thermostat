#pragma once

enum TemperatureTrend
{
    DROP,
    RISE,
    STABLE
};

typedef struct Temperature
{
    float temp;
    float humidity;
    TemperatureTrend trend;
    float coefficient;
} Temperature_t;

class TemperatureListener
{
public:
    virtual void onCurrentTemperature(Temperature_t) = 0;
};
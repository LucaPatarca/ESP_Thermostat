#include <Arduino.h>
#include "temperature.h"

TemperatureController::TemperatureController()
{
    _sensor = new DHT(SENSOR_PIN, DHT22);
    _sensor->begin();
    _lastTrend = TemperatureTrend::STABLE;
    _lastTemp = 0;
    _stableCount = 0;
}

TemperatureTrend TemperatureController::computeTrend()
{
    if (_smoothTemp - _lastTemp > 0.1)
    {
        _lastTemp = _smoothTemp;
        return TemperatureTrend::RISE;
    }
    else if (_lastTemp - _smoothTemp > 0.1)
    {
        _lastTemp = _smoothTemp;
        return TemperatureTrend::DROP;
    }
    else
    {
        if (_lastTrend != TemperatureTrend::STABLE)
        {
            _stableCount++;
            if (_stableCount >= 10)
            {
                _lastTemp = _smoothTemp;
                _stableCount = 0;
                return TemperatureTrend::STABLE;
            }
        }
    }
    return _lastTrend;
}

float TemperatureController::computeCoefficient()
{
    if (_smoothTemp > _lastTemp)
        return _smoothTemp - _lastTemp;
    else
        return _lastTemp - _smoothTemp;
}

void TemperatureController::handle()
{
    if (millis() > _updateTime)
    {
        _smoothTemp = smoothe(_sensor->readTemperature(), _smoothTemp);
        float humidity = _sensor->readHumidity();

        float coefficient = computeCoefficient();
        _lastTrend = computeTrend();

        //TODO arrotondare meglio
        float rounded = static_cast<float>(static_cast<int>(_smoothTemp * 10.)) / 10.;

#ifdef TEMPERATURE_DEBUG
        String s[] = {"Drop", "Rise", "Stable"};
        Serial.printf("Temp: %f\nRounded: %f\nTrend: %s\nCoefficient: %f\nHumidity: %f\n\n", _smoothTemp, rounded, s[_lastTrend].c_str(), coefficient, humidity);
#endif

        for (TemperatureListener *listener : _listeners)
        {
            listener->onCurrentTemperature({rounded, humidity, _lastTrend, coefficient});
        }

        _updateTime = millis() + TEMP_EVENT_INTERVAL;
    }
}

float TemperatureController::smoothe(
    const float input,
    const float data)
{
    return (data == 0) ? input : ((data * (SMOOTH_FACTOR - 1) + input) / SMOOTH_FACTOR);
}
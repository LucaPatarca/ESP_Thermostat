#include <temperature.h>
#include <sdebug.h>

TemperatureController::TemperatureController()
    : _sensor(DHT(SENSOR_PIN, DHT22)),
      _lastTrend(TemperatureTrend::STABLE)
{
    _sensor.begin();
}

TemperatureTrend TemperatureController::computeTrend()
{
    if (_smoothTemp - _lastTemp > CHANGE_THRESHOLD)
    {
        _riseCount++;
        if (_riseCount >= RISE_THRESHOLD)
        {
            _lastTemp = _smoothTemp;
            _riseCount = 0;
            _dropCount = 0;
            _stableCount = 0;
            return TemperatureTrend::RISE;
        }
    }
    else if (_lastTemp - _smoothTemp > CHANGE_THRESHOLD)
    {
        _dropCount++;
        if (_dropCount >= DROP_THRESHOLD)
        {
            _lastTemp = _smoothTemp;
            _dropCount = 0;
            _riseCount = 0;
            _stableCount = 0;
            return TemperatureTrend::DROP;
        }
    }
    else
    {
        _stableCount++;
        if (_stableCount >= STABLE_THRESHOLD)
        {
            if (_lastTrend != TemperatureTrend::STABLE)
            {
                _lastTemp = _smoothTemp;
            }
            _stableCount = 0;
            _dropCount = 0;
            _riseCount = 0;
            return TemperatureTrend::STABLE;
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
        _smoothTemp = smoothe(_sensor.readTemperature(), _smoothTemp);
        if (_lastTemp == 0)
            _lastTemp = _smoothTemp;
        float humidity = _sensor.readHumidity();

        float coefficient = computeCoefficient();
        _lastTrend = computeTrend();

        FINFO("setting temperature temp: %f trend: %d coefficient: %f humidity: %f", _smoothTemp, _lastTrend, coefficient, humidity);

        State::Instance().setCurrentTemperature(Temperature_t{_smoothTemp, humidity, _lastTrend, coefficient});

        _updateTime = millis() + TEMP_EVENT_INTERVAL;
    }
}

float TemperatureController::smoothe(
    const float input,
    const float data)
{
    return (data == 0) ? input : ((data * (SMOOTH_FACTOR - 1) + input) / SMOOTH_FACTOR);
}
#include <temperature.h>
#include <sdebug.h>

#define SMOOTH_FACTOR 4
#define SENSOR_PIN D3

#define TEMP_EVENT_INTERVAL 60000 //in milliseconds
#define STABLE_THRESHOLD 10
#define RISE_THRESHOLD 3
#define DROP_THRESHOLD 3
#define CHANGE_THRESHOLD 0.1

TemperatureController::TemperatureController()
    : m_sensor(DHT(SENSOR_PIN, DHT22)),
      m_lastTrend(TemperatureTrend::STABLE)
{
    m_sensor.begin(100);
    delay(200);
}

TemperatureTrend TemperatureController::computeTrend()
{
    if (m_smoothTemp - m_lastTemp > CHANGE_THRESHOLD)
    {
        m_riseCount++;
        if (m_riseCount >= RISE_THRESHOLD)
        {
            m_lastTemp = m_smoothTemp;
            m_riseCount = 0;
            m_dropCount = 0;
            m_stableCount = 0;
            return TemperatureTrend::RISE;
        }
    }
    else if (m_lastTemp - m_smoothTemp > CHANGE_THRESHOLD)
    {
        m_dropCount++;
        if (m_dropCount >= DROP_THRESHOLD)
        {
            m_lastTemp = m_smoothTemp;
            m_dropCount = 0;
            m_riseCount = 0;
            m_stableCount = 0;
            return TemperatureTrend::DROP;
        }
    }
    else
    {
        m_stableCount++;
        if (m_stableCount >= STABLE_THRESHOLD)
        {
            if (m_lastTrend != TemperatureTrend::STABLE)
            {
                m_lastTemp = m_smoothTemp;
            }
            m_stableCount = 0;
            m_dropCount = 0;
            m_riseCount = 0;
            return TemperatureTrend::STABLE;
        }
    }
    return m_lastTrend;
}

float TemperatureController::computeCoefficient()
{
    if (m_smoothTemp > m_lastTemp)
        return m_smoothTemp - m_lastTemp;
    else
        return m_lastTemp - m_smoothTemp;
}

void TemperatureController::handle()
{
    if (millis() > m_updateTime)
    {
        float temp = m_sensor.readTemperature();
        if(isnan(temp)) return;
        m_smoothTemp = smoothe(temp, m_smoothTemp);
        if (m_lastTemp == 0)
            m_lastTemp = m_smoothTemp;
        float humidity = m_sensor.readHumidity();

        float coefficient = computeCoefficient();
        m_lastTrend = computeTrend();

        INFO("setting temperature temp: %.3f trend: %s coefficient: %.3f humidity: %.3f", m_smoothTemp, tempTrendNames[m_lastTrend], coefficient, humidity);

        State::Instance().setCurrentTemperature(Temperature_t{m_smoothTemp, humidity, m_lastTrend, coefficient});

        m_updateTime = millis() + TEMP_EVENT_INTERVAL;
    }
}

float TemperatureController::smoothe(
    const float input,
    const float data)
{
    return (data == 0) ? input : ((data * (SMOOTH_FACTOR - 1) + input) / SMOOTH_FACTOR);
}
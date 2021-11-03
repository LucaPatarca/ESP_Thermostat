#include <Arduino.h>
#include "temperature.h"

TemperatureController::TemperatureController(){
    m_sensor = new DHT(SENSOR_PIN, DHT22);
    m_sensor->begin();
    m_lastTrend = TemperatureTrend::STABLE;
    m_lastTemp = 0;
    m_stableCount = 0;
}

Temperature TemperatureController::getTemperature()
{
    float coefficient;
    m_smoothTemp = smoothe(m_sensor->readTemperature(), m_smoothTemp);
    float humidity = m_sensor->readHumidity();
    if (m_smoothTemp > m_lastTemp)
        coefficient = m_smoothTemp - m_lastTemp;
    else
        coefficient = m_lastTemp - m_smoothTemp;
    if (m_smoothTemp - m_lastTemp > 0.1)
    {
        m_lastTrend = TemperatureTrend::RISE;
        m_lastTemp = m_smoothTemp;
    }
    else if (m_lastTemp - m_smoothTemp > 0.1)
    {
        m_lastTrend = TemperatureTrend::DROP;
        m_lastTemp = m_smoothTemp;
    }
    else
    {
        if (m_lastTrend != TemperatureTrend::STABLE)
        {
            m_stableCount++;
            if (m_stableCount >= 5)
            {
                m_lastTrend = TemperatureTrend::STABLE;
                m_lastTemp = m_smoothTemp;
                m_stableCount = 0;
            }
        }
    }

    //TODO arrotondare meglio
    float rounded = static_cast<float>(static_cast<int>(m_lastTemp * 10.)) / 10.;

    #ifdef TEMPERATURE_DEBUG
    String s[] = {"Drop", "Rise", "Stable"};
    Serial.printf("Temp: %f\nRounded: %f\nTrend: %s\nCoefficient: %f\nHumidity: %f\n\n", m_smoothTemp, rounded, s[m_lastTrend].c_str(), coefficient, humidity);
    #endif


    return {rounded, humidity, m_lastTrend, coefficient};
}

float TemperatureController::smoothe(
  const float input,
  const float data
) {
  return (data == 0) ? input :
    ((data * (SMOOTH_FACTOR - 1) + input) / SMOOTH_FACTOR);
}
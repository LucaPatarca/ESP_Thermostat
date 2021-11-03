#include "hwio.h"
#include <Wire.h>

HWIOController::HWIOController()
{
    m_display = new Adafruit_SSD1306(128, 64, &Wire, -1);

    if (!m_display->begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
    }
    delay(1500);
}

void HWIOController::setDisplay(int size){
    m_display->clearDisplay();
    m_display->setTextSize(size);
    m_display->setTextColor(WHITE);
    m_display->setCursor(0,0);
}

void HWIOController::setBoilerState(bool state)
{
    if (m_boilerState != state)
    {
        m_needsUpdate = true;
    }
    m_boilerState = state;
}

void HWIOController::setPowerState(bool state)
{
    if (state != m_powerState)
    {
        m_needsUpdate = true;
    }
    m_powerState = state;
}

void HWIOController::setTemperature(Temperature temp)
{
    if (temp.temp != m_temp.temp || temp.humidity != m_temp.humidity || temp.trend != m_temp.trend)
    {
        m_needsUpdate = true;
    }
    m_temp = temp;
}

void HWIOController::setTargetTemperature(float temp)
{
    if (temp != m_targetTemp)
    {
        m_needsUpdate = true;
    }
    m_targetTemp = temp;
}

void HWIOController::update()
{
    if (!m_needsUpdate)
    {
        return;
    }
    setDisplay(3);
    m_display->printf("%.1fC\n", m_temp.temp);
    m_display->setTextSize(2);
    if(m_powerState)
        m_display->printf("%.1fC    ",m_targetTemp);
    else
        m_display->printf("--.-C    ");
    String trend = "";
    switch (m_temp.trend)
    {
    case TemperatureTrend::RISE:
        trend = "R";
        break;
    case TemperatureTrend::DROP:
        trend = "D";
        break;
    case TemperatureTrend::STABLE:
        trend = "S";
        break;
    default:
        break;
    }
    m_display->print(trend);
    m_display->printf("\n%.1f%%    ", m_temp.humidity);
    if(m_boilerState)
        m_display->print("*");
    m_display->display();
    m_needsUpdate = false;
}

void HWIOController::wifiConnecting()
{
    setDisplay(2);
    m_display->println("CONNECTING");
    m_display->println(" WIFI...");
    m_display->display();
}

void HWIOController::wifiConnected()
{
    setDisplay(2);
    m_display->println("  WIFI");
    m_display->println("CONNECTED");
    m_display->display();
}

void HWIOController::updateProgress(float progress)
{
    if (millis() > m_nextUpdate)
    {
        setDisplay(2);
        m_display->println("UPDATING");
        m_display->printf("   %u%%", (int)progress);
        m_display->display();
        m_nextUpdate = millis() + 500;
    }
}

void HWIOController::updateComplete()
{
    setDisplay(2);
    m_display->println(" UPDATE");
    m_display->println("COMPLETE");
    m_display->display();
}
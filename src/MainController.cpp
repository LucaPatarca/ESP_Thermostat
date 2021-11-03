#include <MainController.h>

MainController::MainController()
{
    m_boilerState = false;
    
    m_hwio.wifiConnecting();
    m_wifi.connect();
    m_hwio.wifiConnected();

    m_alexa.setup();

    m_ota = new OTAController(&m_alexa, &m_hwio);

    m_updateTime = millis()+UPDATE_INTERVAL;
}

void MainController::setBoilerState(bool state)
{
    if (state != m_boilerState)
    {
        m_boilerState = state;
    }
}

void MainController::handle()
{
    m_alexa.handle();
    m_ota->handle();

    if (millis() > m_updateTime)
    {
        Temperature temp = m_temperature.getTemperature();
        TCase tempCase = getTempCase(temp);
        switch (tempCase)
        {
        case TCase::OFF_BELOW_SAFE_RANGE:
        case TCase::BELOW_LOW_RANGE:
        case TCase::BELOW_RANGE_HIGH_NOT_RISING:
        case TCase::BELOW_TARGET_DROPPING:
            setBoilerState(true);
            break;

        case TCase::OFF_ABOVE_SAFE_RANGE:
        case TCase::BELOW_RANGE_HIGH_RISING:
        case TCase::BELOW_TARGET_NOT_DROPPING:
        case TCase::ABOVE_TARGET:
            setBoilerState(false);
            break;

        default:
            break;
        }

        m_alexa.updateCurrentTemperature(temp.temp, temp.humidity);

        m_hwio.setBoilerState(m_boilerState);
        m_hwio.setTemperature(temp);
        m_hwio.update();

        m_updateTime = millis() + UPDATE_INTERVAL;
    }
}

TCase MainController::getTempCase(Temperature temp)
{
    float target = m_alexa.getTargetTemperature();
    if (!m_alexa.isOn())
    {
        if (temp.temp < SAFE_TEMP)
        {
            return TCase::OFF_BELOW_SAFE_RANGE;
        }
        else if (temp.temp > SAFE_TEMP + TEMP_RANGE_LOW)
        {
            return TCase::OFF_ABOVE_SAFE_RANGE;
        }else{
            return TCase::OFF_IN_SAFE_RANGE;
        }
    }
    else if (temp.temp < target - TEMP_RANGE_LOW)
    {
        return TCase::BELOW_LOW_RANGE;
    }
    else if (temp.temp < target - TEMP_RANGE_HIGH)
    {
        if (temp.trend == TemperatureTrend::RISE)
        {
            return TCase::BELOW_RANGE_HIGH_RISING;
        }
        else
        {
            return TCase::BELOW_RANGE_HIGH_NOT_RISING;
        }
    }
    else if (temp.temp < target)
    {
        if (temp.trend == TemperatureTrend::DROP)
        {
            return TCase::BELOW_TARGET_DROPPING;
        }
        else
        {
            return TCase::BELOW_TARGET_NOT_DROPPING;
        }
    }
    else
    {
        return TCase::ABOVE_TARGET;
    }
}
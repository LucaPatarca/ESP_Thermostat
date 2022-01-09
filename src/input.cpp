#ifndef NO_PHISICAL_INPUT

#include <input.h>

InputController::InputController()
{
    pinMode(MODE_PIN, INPUT);
}

void InputController::onTargetTemperature(float temp)
{
    _lastTemp = temp;
}

void InputController::onPowerState(bool) {}

void InputController::onThermostatMode(Mode mode)
{
    _lastMode = mode;
}

void InputController::onSetSetting(const String, String) {}

void InputController::handle()
{
    if (millis() > _lastEventTime + EVENT_INTERVAL)
    {
        if (digitalRead(MODE_PIN) == HIGH)
        {
            _lastMode = static_cast<Mode>((_lastMode + 1) % 3);
            for (StateListener *listener : _listeners)
            {
                listener->onThermostatMode(_lastMode);
            }
            if (_lastMode == Mode::OFF)
            {
                for (StateListener *listener : _listeners)
                {
                    listener->onPowerState(false);
                }
            } else if (_lastMode == Mode::ON)
            {
                for (StateListener *listener : _listeners)
                {
                    listener->onPowerState(true);
                }
            }
#ifdef INPUT_DEBUG
            printf("selected mode: %d\n", _lastMode);
#endif
            _lastEventTime = millis();
        }
        else if (digitalRead(TEMP_UP) == HIGH)
        {
            _lastTemp = _lastTemp + TEMP_CHANGE;
            for (StateListener *listener : _listeners)
            {
                listener->onTargetTemperature(_lastTemp);
            }
#ifdef INPUT_DEBUG
            printf("Temp +\n");
#endif
            _lastEventTime = millis();
        }
        else if (digitalRead(TEMP_DOWN) == HIGH)
        {
            _lastTemp = _lastTemp - TEMP_CHANGE;
            for (StateListener *listener : _listeners)
            {
                listener->onTargetTemperature(_lastTemp);
            }
#ifdef INPUT_DEBUG
            printf("Temp -\n");
#endif
            _lastEventTime = millis();
        }
    }
}

#endif
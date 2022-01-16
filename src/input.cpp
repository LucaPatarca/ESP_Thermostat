#ifndef NO_PHISICAL_INPUT

#include <input.h>
#include <Arduino.h>

InputController::InputController()
    : m_state(State::Instance())
{
    pinMode(MODE_PIN, INPUT);
    pinMode(TEMP_UP_PIN, INPUT);
    pinMode(TEMP_DOWN_PIN, INPUT);
}

void InputController::handle()
{
    //TODO check for multiple buttons pressed at the same time, this can be also used to activate special modes such as the sos mode
    if (millis() > _lastEventTime + EVENT_INTERVAL)
    {
        if (digitalRead(MODE_PIN) == HIGH)
        {
            m_state.setThermostatMode(Cause::MANUAL, static_cast<Mode>((m_state.getThermostatMode() + 1) % 3));
#ifdef INPUT_DEBUG
            printf("selected mode: %d\n", _lastMode);
#endif
            _lastEventTime = millis();
        }
        else if (digitalRead(TEMP_UP_PIN) == HIGH)
        {
            m_state.setTargetTemperature(Cause::MANUAL, m_state.getTargetTemperature() + TEMP_CHANGE);
#ifdef INPUT_DEBUG
            printf("Temp +\n");
#endif
            _lastEventTime = millis();
        }
        else if (digitalRead(TEMP_DOWN_PIN) == HIGH)
        {
            m_state.setTargetTemperature(Cause::MANUAL, m_state.getTargetTemperature() - TEMP_CHANGE);
#ifdef INPUT_DEBUG
            printf("Temp -\n");
#endif
            _lastEventTime = millis();
        }
    }
}

#endif
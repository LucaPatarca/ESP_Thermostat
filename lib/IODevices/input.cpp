#ifndef NO_BUTTON_INPUT

#include <input.h>
#include <Arduino.h>
#include <sdebug.h>

#define EVENT_INTERVAL 300      //in milliseconds
#define TEMP_CHANGE 0.1         //in degrees
#define MODE_PIN D6
#define TEMP_UP_PIN D8
#define TEMP_DOWN_PIN D7

InputController::InputController()
    : m_state(State::Instance())
{
    pinMode(MODE_PIN, INPUT);
    pinMode(TEMP_UP_PIN, INPUT);
    pinMode(TEMP_DOWN_PIN, INPUT);
}

void InputController::handle()
{
    if (millis() > _lastEventTime + EVENT_INTERVAL)
    {
        int modePin = digitalRead(MODE_PIN);
        int tempUpPin = digitalRead(TEMP_UP_PIN);
        int tempDownPin = digitalRead(TEMP_DOWN_PIN);
        if(modePin + tempUpPin + tempDownPin > 1)
        {
            WARN("multiple buttons pressed together, nothing will happen");
            _lastEventTime = millis();
            return;
        }
        if (modePin == HIGH)
        {
            INFO("mode button pressed");
            m_state.setThermostatMode(Cause::MANUAL, static_cast<Mode>((m_state.getThermostatMode() + 1) % 3));
            _lastEventTime = millis();
        }
        else if (tempUpPin == HIGH)
        {
            INFO("temperature + pressed");
            m_state.setTargetTemperature(Cause::MANUAL, m_state.getTargetTemperature() + TEMP_CHANGE);
            _lastEventTime = millis();
        }
        else if (tempDownPin == HIGH)
        {
            INFO("temperature - pressed");
            m_state.setTargetTemperature(Cause::MANUAL, m_state.getTargetTemperature() - TEMP_CHANGE);
            _lastEventTime = millis();
        }
    }
}

#endif
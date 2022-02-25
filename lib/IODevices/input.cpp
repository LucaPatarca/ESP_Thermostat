#ifndef NO_BUTTON_INPUT

#include <input.h>
#include <Arduino.h>
#include <sdebug.h>
#include <display.h>

#define EVENT_INTERVAL 300        // in milliseconds
#define MODE_CHANGE_INTERVAL 1500 // in milliseconds
#define TEMP_CHANGE 0.1           // in degrees
#define MODE_PIN D6
#define TEMP_UP_PIN D8
#define TEMP_DOWN_PIN D7
#define RESET_THRESHOLD 20

InputController::InputController()
    : m_state(State::Instance())
{
    pinMode(MODE_PIN, INPUT);
    pinMode(TEMP_UP_PIN, INPUT);
    pinMode(TEMP_DOWN_PIN, INPUT);
}

void InputController::abortReset()
{
    DisplayController::Instance().notify("Annullato.", 2);
    m_resetPressedCount = 0;
}

void InputController::handle()
{
    if (millis() > m_lastEventTime + EVENT_INTERVAL)
    {
        int modePin = digitalRead(MODE_PIN);
        int tempUpPin = digitalRead(TEMP_UP_PIN);
        int tempDownPin = digitalRead(TEMP_DOWN_PIN);
        if (modePin + tempUpPin + tempDownPin > 1)
        {
            WARN("multiple buttons pressed together");
            m_lastEventTime = millis();
            if (m_resetPressedCount == RESET_THRESHOLD)
            {
                DisplayController::Instance().notify("Il termostato\nsta per essere\nresettato...", 5);
                return;
            }
            m_resetPressedCount++;
        }
        else if (modePin == HIGH)
        {
            INFO("mode button pressed");
            m_state.setThermostatMode(Cause::MANUAL, static_cast<Mode>((m_state.getThermostatMode() + 1) % 3));
            if (m_resetPressedCount == RESET_THRESHOLD)
                abortReset();
            m_lastEventTime = millis();
        }
        else if (tempUpPin == HIGH)
        {
            INFO("temperature + pressed");
            m_state.setTargetTemperature(Cause::MANUAL, m_state.getTargetTemperature() + TEMP_CHANGE);
            if (m_resetPressedCount == RESET_THRESHOLD)
                abortReset();
            m_lastEventTime = millis();
        }
        else if (tempDownPin == HIGH)
        {
            INFO("temperature - pressed");
            m_state.setTargetTemperature(Cause::MANUAL, m_state.getTargetTemperature() - TEMP_CHANGE);
            if (m_resetPressedCount == RESET_THRESHOLD)
                abortReset();
            m_lastEventTime = millis();
        }
        else if (m_resetPressedCount == RESET_THRESHOLD)
        {
            DisplayController::Instance().notify("Premere un \ntasto per\nannullare...", 10);
            if (millis() > m_lastEventTime + 10000)
            {
                m_state.factoryReset();
                m_resetPressedCount = 0;
            }
        }
        else
        {
            m_resetPressedCount = 0;
        }
    }
}

#endif
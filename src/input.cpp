#include <input.h>

InputController::InputController()
{
    pinMode(PROGRAM_PIN, INPUT);
}

void InputController::onTargetTemperature(float temp)
{
    _lastTemp = temp;
}

void InputController::onPowerState(bool){}

void InputController::onThermostatMode(Mode mode)
{
    _lastMode = mode;
}

void InputController::onSetSetting(const String, String){}


void InputController::handle()
{
    if(digitalRead(PROGRAM_PIN) == HIGH && _lastMode != Mode::PROGRAM){
        for(StateListener *listener : _listeners){
            listener->onThermostatMode(Mode::PROGRAM);
        }
    }
}
#include <program.h>

ProgramController::ProgramController()
{
    _lastDay = _time.getDayOfWeek();
    _lastTime = _time.getTime();
}

void ProgramController::handle()
{
    if (_mode == Mode::PROGRAM)
    {
        int day = _time.getDayOfWeek();
        int time = _time.getTime();
        if (time != _lastTime || day != _lastDay)
        {
            for (StateListener *listener : _listeners)
            {
                float target = _program.days[day].temps[time];
                if (target > 0)
                {
                    listener->onTargetTemperature(target);
                    listener->onPowerState(true);
                }
                else
                {
                    listener->onPowerState(false);
                }
            }
            _lastDay = day;
            _lastTime = time;
        }
    }
}

void ProgramController::onPowerState(bool state)
{
    //nop
}

void ProgramController::onTargetTemperature(float temp)
{
    //nop
}

void ProgramController::onThermostatMode(Mode mode)
{
    _mode = mode;
    _lastDay = -1;
    _lastTime = -1;
}

#pragma once

#include <EventEmitter.h>
#include <StateListener.h>
#include <ntime.h>

typedef struct DayProgram{
    float temps[48];
} DayProgram_t;

typedef struct WeekProgram {
    DayProgram_t days[7];
} WeekProgram_t;

class ProgramController: public EventEmitter<StateListener>, public StateListener
{
private:
    WeekProgram _program;
    Mode _mode;
    TimeController _time;
    int _lastDay;
    int _lastTime;
public:
    ProgramController();

    void onPowerState(bool) override;
    void onTargetTemperature(float) override;
    void onThermostatMode(Mode) override;

    void handle();
};
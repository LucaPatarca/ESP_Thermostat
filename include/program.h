#pragma once

#include <EventEmitter.h>
#include <StateListener.h>
#include <ntime.h>

#define PROGRAM_DEBUG

typedef struct DayProgram{
    unsigned char temps[48];
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

    float getTemperature(int, int);
    void putTemperature(int, int, float);

    WeekProgram loadProgram();
    bool saveProgram();
public:
    ProgramController();

    void onPowerState(bool) override;
    void onTargetTemperature(float) override;
    void onThermostatMode(Mode) override;
    void onSetSetting(String, String) override;

    void handle();
};
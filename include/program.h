#pragma once

#include <EventEmitter.h>
#include <StateListener.h>
#include <ntime.h>

// #define PROGRAM_DEBUG

typedef struct DayProgram{
    unsigned char temps[48];
} DayProgram_t;

typedef struct WeekProgram {
    DayProgram_t days[7];
} WeekProgram_t;

typedef struct ScheduleChange{
    int days[7];
    int fromTime;
    int toTime;
    float temp;
} ScheduleChange_t;

class ProgramController: public EventEmitter<StateListener>, public StateListener
{
private:
    WeekProgram _program;
    Mode _mode;
    int _lastDay;
    int _lastTime;

    float getTemperature(int, int);
    void putTemperature(int, int, float);

    WeekProgram loadProgram();
    bool saveProgram();

    ScheduleChange_t createEmptyChange();
    ScheduleChange_t parseChange(String value);
    void addScheduleChange(ScheduleChange_t change);
    void removeScheduleChange(ScheduleChange_t change);

    void applyProgram();
public:
    ProgramController();

    void onPowerState(bool) override;
    void onTargetTemperature(float) override;
    void onThermostatMode(Mode) override;
    void onSetSetting(String, String) override;

    void handle();
};
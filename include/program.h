#pragma once

#include <state.h>

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

class ProgramController
{
private:
    State &m_state;
    WeekProgram _program;
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

    ProgramController();
public:
    ProgramController(ProgramController&) = delete;

    static ProgramController& Instance(){
        static ProgramController controller;
        return controller;
    }

    void thermostatModeChanged();
    void onSetSetting(const String&, String&);

    void handle();
};
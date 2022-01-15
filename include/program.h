#pragma once

#include <state.h>

// #define PROGRAM_DEBUG

typedef struct {
    unsigned char temps[48];
} DayProgram_t;

typedef struct {
    DayProgram_t days[7];
} WeekProgram_t;

typedef struct {
    int days[7];
    int fromTime;
    int toTime;
    float temp;
} ScheduleChange_t;

class ProgramController
{
private:
    State &m_state;
    WeekProgram_t _program;
    int m_lastDay;
    int m_lastTime;

    float getTemperature(int, int);
    void putTemperature(int, int, float);

    void loadProgram();
    bool saveProgram();

    int parseChange(ScheduleChange_t&, String &value);
    void addScheduleChange(const ScheduleChange_t &change);
    void removeScheduleChange(const ScheduleChange_t &change);

    void applyProgram();

    ProgramController();
public:
    ProgramController(const ProgramController&) = delete;

    static ProgramController& Instance(){
        static ProgramController controller;
        return controller;
    }

    void thermostatModeChanged();
    void onSetSetting(const String&, String&);

    void handle();
};
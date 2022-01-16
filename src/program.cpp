#include <program.h>
#include <ESP_EEPROM.h>
#include <ArduinoJson.h>

//TODO add macro for debug purpose in a global .h file

ProgramController::ProgramController()
    : m_state(State::Instance())
{
    m_lastDay = -1;
    m_lastTime = -1;
#ifdef PROGRAM_DEBUG
    Serial.printf("[ProgramController] EEPROM.begin(%d)\n", sizeof(WeekProgram_t));
#endif
    EEPROM.begin(sizeof(WeekProgram_t));
    loadProgram();
}

float ProgramController::getTemperature(int day, int time)
{
    float temp = (_program.days[day].temps[time] / 10) + 10;
    return temp <= 10 ? 0 : temp;
}

void ProgramController::putTemperature(int day, int time, float temp)
{
    unsigned char value;
    if (temp < 10)
    {
        value = 0;
    }
    else
    {
        value = static_cast<unsigned char>((temp - 10) * 10);
    }

#ifdef PROGRAM_DEBUG
    Serial.printf("putting %d to %d - %d\n", value, day, time);
#endif

    _program.days[day].temps[time] = value;
}

void ProgramController::loadProgram()
{
#ifdef PROGRAM_DEBUG
    Serial.printf("ProgramController::loadProgram()\n");
#endif
    _program = EEPROM.get(0, _program);
}

bool ProgramController::saveProgram()
{
#ifdef PROGRAM_DEBUG
    Serial.printf("ProgramController::saveProgram()\n");
#endif
    EEPROM.put(0, _program);
    return EEPROM.commit();
}

void ProgramController::applyProgram()
{
#ifdef PROGRAM_DEBUG
    Serial.printf("ProgramController::applyProgram()\n");
#endif
    float target = getTemperature(m_lastDay, m_lastTime);
        if (target > 0)
        {
            m_state.setTargetTemperature(Cause::SCHEDULE, target);
            m_state.setPowerState(Cause::SCHEDULE, true);
        }
        else
        {
            m_state.setPowerState(Cause::SCHEDULE, false);
        }

#ifdef PROGRAM_DEBUG
    Serial.printf("[ProgramController] day: %d, time: %d, formatted: %s, target: %.1f\n", m_lastDay, m_lastTime, m_state.getFormattedTime().c_str(), target);
    for (int d = 0; d < 7; d++)
    {
        Serial.printf("day: %d\n[ ", d);
        for (int t = 0; t < 48; t++)
        {
            Serial.printf("%d: %.1f, ", t, getTemperature(d, t));
        }
        Serial.printf(" ]\n");
    }
#endif
}

void ProgramController::handle()
{
    if (m_state.getThermostatMode() == Mode::PROGRAM)
    {
        Time_t time = m_state.getTime();
        int programTime = (time.hour*2)+(time.minutes>=30?1:0);
        if (programTime != m_lastTime || time.day != m_lastDay)
        {
            m_lastDay = time.day;
            m_lastTime = programTime;
            applyProgram();
        }
    }
}

void ProgramController::thermostatModeChanged()
{
    //reset
    m_lastDay = -1;
    m_lastTime = -1;
}

int ProgramController::parseChange(ScheduleChange_t& change, String &value)
{
    DynamicJsonDocument doc(128);
    char *noCopy = (char *)value.c_str();
    DeserializationError error = deserializeJson(doc, noCopy, value.length());

    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return 1;
    }

    JsonArray days = doc["days"];
    for (int d : days)
    {
        change.days[d] = 1;
    }

    change.fromTime = doc["fromHour"];
    change.toTime = doc["toHour"];

    change.temp = doc["temp"]; // 20.8

#ifdef PROGRAM_DEBUG
    Serial.printf("days:[");
    for (int d : days)
    {
        Serial.printf("%d, ", d);
    }
    Serial.printf("], from: %d, to: %d, %.1f C\n", change.fromTime, change.toTime, change.temp);
#endif

    return 0;
}

void ProgramController::addScheduleChange(const ScheduleChange_t &change)
{
    for (int i = 0; i < 7; i++)
    {
        if (change.days[i])
        {
            for (int t = change.fromTime; t <= change.toTime; t++)
            {
                putTemperature(i, t, change.temp);
            }
        }
    }

    saveProgram();
    loadProgram();
    applyProgram();
}

void ProgramController::removeScheduleChange(const ScheduleChange_t &change)
{
    for (int i = 0; i < 7; i++)
    {
        if (change.days[i])
        {
            for (int t = change.fromTime; t <= change.toTime; t++)
            {
                putTemperature(i, t, 0);
            }
        }
    }

    saveProgram();
    loadProgram();
    applyProgram();
}

void ProgramController::onSetSetting(const String &key, String &value)
{
    if (key == "ScheduleChange")
    {
        ScheduleChange_t change;
        int error = parseChange(change, value);
        if(!error)
            addScheduleChange(change);
    }
    else if (key == "ScheduleChangeRemove")
    {
        ScheduleChange_t change;
        int error = parseChange(change, value);
        if(!error)
            removeScheduleChange(change);
    }
}

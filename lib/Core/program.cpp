#include <program.h>
#include <ArduinoJson.h>
#include <sdebug.h>

ProgramController::ProgramController()
    : m_state(State::Instance()),
      m_program(m_state.getConfig().program),
      m_lastDay(-1),
      m_lastTime(-1)
{
}

float ProgramController::getTemperature(int day, int time)
{
    float temp = (m_program.days[day].temps[time] / 10) + 10;
    return temp <= 10 ? 0 : temp;
}

void ProgramController::putTemperature(int day, int time, float temp)
{
    uint8_t value;
    if (temp < 10)
    {
        value = 0;
    }
    else
    {
        value = static_cast<uint8_t>((temp - 10) * 10);
    }
    FINE("putting %d to %d - %d\n", value, day, time);
    m_program.days[day].temps[time] = value;
}

bool ProgramController::saveProgram()
{
    INFO("saving program");
    return m_state.setProgram(m_program);
}

void ProgramController::applyProgram()
{
    INFO("applying program");
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

//logged at level FINE
#if LOG_LEVEL > LOG_LEVEL_INFO
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
        int programTime = (time.hour * 2) + (time.minutes >= 30 ? 1 : 0);
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

int ProgramController::parseChange(ScheduleChange_t &change, String &value)
{
    DynamicJsonDocument doc(128);
    char *noCopy = (char *)value.c_str();
    DeserializationError error = deserializeJson(doc, noCopy, value.length());

    if (error)
    {
        ERROR("deserializeJson() failed: %s", String(error.f_str()).c_str());
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

//logged at level FINE
#if LOG_LEVEL > LOG_LEVEL_INFO
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
    INFO("adding schedule change");
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
    applyProgram();
}

void ProgramController::removeScheduleChange(const ScheduleChange_t &change)
{
    INFO("removing schedule change");
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
    applyProgram();
}

void ProgramController::onSetSetting(const String &key, String &value)
{
    if (key == "ScheduleChange")
    {
        ScheduleChange_t change;
        int error = parseChange(change, value);
        if (!error)
            addScheduleChange(change);
    }
    else if (key == "ScheduleChangeRemove")
    {
        ScheduleChange_t change;
        int error = parseChange(change, value);
        if (!error)
            removeScheduleChange(change);
    }
}

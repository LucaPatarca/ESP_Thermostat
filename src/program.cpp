#include <program.h>
#include <ESP_EEPROM.h>
#include <ArduinoJson.h>

ProgramController::ProgramController()
{
    _lastDay = -1;
    _lastTime = -1;
#ifdef PROGRAM_DEBUG
    Serial.printf("[ProgramController] EEPROM.begin(%d)\n", sizeof(WeekProgram));
#endif
    EEPROM.begin(sizeof(WeekProgram));
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

WeekProgram ProgramController::loadProgram()
{
#ifdef PROGRAM_DEBUG
    Serial.printf("ProgramController::loadProgram()\n");
#endif
    return EEPROM.get(0, _program);
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
    float target = getTemperature(_lastDay, _lastTime);
    for (StateListener *listener : _listeners)
    {
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

#ifdef PROGRAM_DEBUG
    Serial.printf("[ProgramController] day: %d, time: %d, formatted: %s, target: %.1f\n", _lastDay, _lastTime, _time.getFormattedTime().c_str(), target);
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
    if (_mode == Mode::PROGRAM)
    {
        Time_t time = Time->getTime();
        int programTime = (time.hour*2)+(time.minutes>=30?1:0);
        if (programTime != _lastTime || time.day != _lastDay)
        {
            _lastDay = time.day;
            _lastTime = programTime;
            applyProgram();
        }
    }
}

void ProgramController::onPowerState(bool state)
{
    //nop
}

void ProgramController::onTargetTemperature(float temp)
{
    if (_mode == Mode::PROGRAM)
    {
        for (StateListener *listener : _listeners)
        {
            listener->onThermostatMode(Mode::ON);
            listener->onPowerState(true);
        }
        _mode = Mode::ON;
    }
}

void ProgramController::onThermostatMode(Mode mode)
{
    _mode = mode;
    _lastDay = -1;
    _lastTime = -1;
}

ScheduleChange_t ProgramController::createEmptyChange()
{
    return {{0, 0, 0, 0, 0, 0, 0}, 0, 1, 0};
}

ScheduleChange_t ProgramController::parseChange(String value)
{
    DynamicJsonDocument doc(128);
    char *noCopy = (char *)value.c_str();
    DeserializationError error = deserializeJson(doc, noCopy, value.length());

    ScheduleChange_t parsed = createEmptyChange();

    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return parsed;
    }

    JsonArray days = doc["days"];
    for (int d : days)
    {
        parsed.days[d] = 1;
    }

    parsed.fromTime = doc["fromHour"];
    parsed.toTime = doc["toHour"];

    parsed.temp = doc["temp"]; // 20.8

#ifdef PROGRAM_DEBUG
    Serial.printf("days:[");
    for (int d : days)
    {
        Serial.printf("%d, ", d);
    }
    Serial.printf("], from: %d, to: %d, %.1f C\n", parsed.fromTime, parsed.toTime, parsed.temp);
#endif

    return parsed;
}

void ProgramController::addScheduleChange(ScheduleChange_t change)
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

void ProgramController::removeScheduleChange(ScheduleChange_t change)
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

void ProgramController::onSetSetting(String key, String value)
{
    if (key == "ScheduleChange")
    {
        ScheduleChange_t change = parseChange(value);
        addScheduleChange(change);
    }
    else if (key == "ScheduleChangeRemove")
    {
        ScheduleChange_t change = parseChange(value);
        removeScheduleChange(change);
    }
}

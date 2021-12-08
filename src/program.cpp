#include <program.h>
#include <ESP_EEPROM.h>
#include <ArduinoJson.h>

ProgramController::ProgramController()
{
    _lastDay = _time.getDayOfWeek();
    _lastTime = _time.getTime();
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
    Serial.printf("putting %d to %d - %d\n", value, day, time);
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
                float target = getTemperature(time, day);
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

#ifdef PROGRAM_DEBUG
            Serial.printf("[ProgramController] day: %d time: %d\n", day, time);
            for (int d = 0; d < 7; d++)
            {
                Serial.printf("day: %d\n[ ", d);
                for (int t = 0; t < 48; t++)
                {
                    Serial.printf("%.1f, ", getTemperature(d, t));
                }
                Serial.printf(" ]\n");
            }
#endif
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

void ProgramController::onSetSetting(String key, String value)
{
    if (key =="ProgramEvent")
    {
        DynamicJsonDocument doc(128);
        char *noCopy = (char*)value.c_str();
        DeserializationError error = deserializeJson(doc, noCopy, value.length());

        if (error)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
        }

        int fromDay = doc["from"]["day"];   // 5
        int fromTime = doc["from"]["time"]; // 29

        int toDay = doc["to"]["day"];   // 5
        int toTime = doc["to"]["time"]; // 37

        float temp = doc["temp"]; // 20.8

#ifdef PROGRAM_DEBUG
        Serial.printf("from: %d - %d, to: %d - %d, %.1f\n", fromDay, fromTime, toDay, toTime, temp);
#endif

        for (int d = fromDay; d <= toDay; d++)
        {
            for (int t = fromTime; t <= toTime; t++)
            {
                putTemperature(d,t,temp);
            }
        }

        saveProgram();
        loadProgram();
    }
}

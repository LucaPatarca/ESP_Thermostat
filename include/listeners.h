#pragma once

#include <WString.h>

class SettingsListener
{
public:
    virtual void onSetSetting(const String &, String &);
};

enum Cause
{
    NONE,
    ALEXA,
    MANUAL,
    SCHEDULE,
    AUTO
};

class StateListener
{
public:
    virtual void targetTemperatureChanged(Cause) = 0;
    virtual void powerStateChanged(Cause) = 0;
    virtual void thermostatModeChanged(Cause) = 0;
    virtual void boilerStateChanged() = 0;
    virtual void currentTemperatureChanged() = 0;
    virtual void wifiStatusChanged() = 0;
};

enum UpdateEventType
{
    START,
    END,
    PROGRESS,
    ERROR
};

typedef struct UpdateEvent
{
    UpdateEventType type;
    float progress;
    const char *message;
    bool operator==(const UpdateEvent &rhs) const
    {
        return this->type == rhs.type && this->progress == rhs.progress && this->message == rhs.message;
    }
    bool operator!=(const UpdateEvent &rhs) const
    {
        return this->type != rhs.type && this->progress != rhs.progress && this->message != rhs.message;
    }
} UpdateEvent_t;

class UpdateListener
{
public:
    virtual void onUpdateEvent(UpdateEvent_t &) = 0;
};
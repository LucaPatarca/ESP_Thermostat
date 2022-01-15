#pragma once

#include <WString.h>
#include <cstdio>

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
    //TODO probably not ok
    const char *message;

    String toString() const {
        char s[64];
        const char *types[] = {"Start", "End", "Progress", "Error"};
        sprintf(s, "UpdateEvent{type: %s, progress: %.3f, message: %s}", types[type], progress, message);
        return s;
    }

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
    virtual void onUpdateEvent(const UpdateEvent_t &) = 0;
};
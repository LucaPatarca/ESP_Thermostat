#pragma once

enum UpdateEventType{
    START,
    END,
    PROGRESS,
    ERROR
};

typedef struct UpdateEvent{
    UpdateEventType type;
    float progress;
    const char *message;
} UpdateEvent_t;

class UpdateListener
{
public:
    virtual void onUpdateEvent(UpdateEvent) = 0;
};
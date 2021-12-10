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
    bool operator ==(const UpdateEvent &rhs) const {
        return this->type == rhs.type && this->progress == rhs.progress && this->message == rhs.message;
    }
    bool operator !=(const UpdateEvent &rhs) const {
        return this->type != rhs.type && this->progress != rhs.progress && this->message != rhs.message;
    }
} UpdateEvent_t;

class UpdateListener
{
public:
    virtual void onUpdateEvent(UpdateEvent) = 0;
};
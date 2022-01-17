#ifndef NO_BUTTON_INPUT

#include <state.h>

class InputController
{
private:
    unsigned long _lastEventTime;
    State &m_state;

    InputController();
public:
    InputController(const InputController&) = delete;

    static InputController& Instance(){
        static InputController controller;
        return controller;
    }

    void handle();
};

#define IF_INPUT_ENABLED(...) __VA_ARGS__
#else
#define IF_INPUT_ENABLED(...)
#endif
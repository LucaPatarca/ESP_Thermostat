#ifndef NO_BUTTON_INPUT

#include <state.h>

class InputController
{
private:
    unsigned long m_lastEventTime;
    State &m_state;
    uint32_t m_resetPressedCount;

    void abortReset();

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
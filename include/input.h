#ifndef NO_PHISICAL_INPUT

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
#endif
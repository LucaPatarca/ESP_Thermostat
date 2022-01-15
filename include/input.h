#ifndef NO_PHISICAL_INPUT

#include <state.h>

#define EVENT_INTERVAL 200      //in milliseconds
#define TEMP_CHANGE 0.1         //in degrees
#define MODE_PIN D6
#define TEMP_UP_PIN D8
#define TEMP_DOWN_PIN D7

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
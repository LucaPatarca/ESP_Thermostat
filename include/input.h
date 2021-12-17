#ifndef NO_PHISICAL_INPUT

#include <EventEmitter.h>
#include <StateListener.h>

#define PROGRAM_PIN D8

class InputController: public EventEmitter<StateListener>, public StateListener
{
private:
    float _lastTemp;
    Mode _lastMode;
public:
    InputController();

    void onTargetTemperature(float) override;
    void onPowerState(bool) override;
    void onThermostatMode(Mode) override;
    void onSetSetting(const String, String) override;

    void handle();
};
#endif
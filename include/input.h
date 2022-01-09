#ifndef NO_PHISICAL_INPUT

#include <EventEmitter.h>
#include <StateListener.h>

#define EVENT_INTERVAL 500      //in milliseconds
#define TEMP_CHANGE 0.1         //in degrees
#define MODE_PIN D6
#define TEMP_UP D8
#define TEMP_DOWN D7

class InputController: public EventEmitter<StateListener>, public StateListener
{
private:
    float _lastTemp;
    Mode _lastMode;
    unsigned long _lastEventTime;
public:
    InputController();

    void onTargetTemperature(float) override;
    void onPowerState(bool) override;
    void onThermostatMode(Mode) override;
    void onSetSetting(const String, String) override;

    void handle();
};
#endif
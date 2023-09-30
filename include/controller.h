#include <listeners.h>
#include <wifi.h>
#include <boiler.h>
#include <display.h>
#include <ota.h>
#include <temperature.h>
#include <program.h>
#include <input.h>
#include <MQTT.h>

class MainController : public StateListener, public SettingsListener, public UpdateListener
{
public:
    MainController(const MainController&) = delete;
    
    static MainController& Instance(){
        static MainController controller;
        return controller;
    }

    void setup();
    void handle();

    void targetTemperatureChanged(Cause) override;
    void powerStateChanged(Cause) override;
    void thermostatModeChanged(Cause) override;
    void boilerStateChanged() override;
    void currentTemperatureChanged() override;
    void wifiStatusChanged() override;

    void onSetSetting(const String&, String&) override;
    void onUpdateEvent(const UpdateEvent_t&) override;

private:
    BoilerController &m_thermostat;
    IF_DISPLAY_ENABLED(DisplayController &m_display;)
    OTAController &m_ota;
    TemperatureController &m_temperature;
    WifiController &m_wifi;
    ProgramController &m_program;
    IF_INPUT_ENABLED(InputController &m_input;)
    State &m_state;
    MQTTController &m_mqtt;

    MainController();
};

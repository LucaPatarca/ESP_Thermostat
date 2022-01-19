#include <controller.h>
#include <sdebug.h>

MainController::MainController()
    : m_thermostat(BoilerController::Instance()),
    IF_DISPLAY_ENABLED(m_display(DisplayController::Instance()),)
    m_alexa(*new AlexaController()),
    m_ota(OTAController::Instance()),
    m_temperature(TemperatureController::Instance()),
    m_wifi(WifiController::Instance()),
    m_program(ProgramController::Instance()),
    IF_INPUT_ENABLED(m_input(InputController::Instance()),)
    m_state(State::Instance())
{
    m_state.addListener(this);
    m_alexa.addSettingListener(this);
}

void MainController::setup()
{
    m_wifi.connect();
    m_ota.setOnUpdateEvent([this](const UpdateEvent_t& event){ onUpdateEvent(event); });
}

void MainController::handle()
{
    m_alexa.handle();
    m_ota.handle();
    m_temperature.handle();
    m_program.handle();
    IF_INPUT_ENABLED(m_input.handle();)
    IF_DISPLAY_ENABLED(m_display.handle();)
}

void MainController::targetTemperatureChanged(Cause cause)
{
    FINE("notify target temperature changed");
    IF_DISPLAY_ENABLED(m_display.targetTemperatureChanged(cause);)
    m_alexa.targetTemperatureChanged(cause);
    m_thermostat.targetTemperatureChanged();
}

void MainController::powerStateChanged(Cause cause)
{
    FINE("notify power state changed");
    IF_DISPLAY_ENABLED(m_display.powerStateChanged();)
    m_alexa.powerStateChanged(cause);
    m_thermostat.powerStateChanged();
}

void MainController::thermostatModeChanged(Cause cause)
{
    FINE("notify thermostat mode changed");
    IF_DISPLAY_ENABLED(m_display.thermostatModeChanged(cause);)
    m_alexa.thermostatModeChanged(cause);
    m_program.thermostatModeChanged();
}

void MainController::boilerStateChanged()
{
    FINE("notify boiler state changed");
    IF_DISPLAY_ENABLED(m_display.boilerStateChanged();)
}

void MainController::currentTemperatureChanged()
{
    FINE("notify current temperature changed");
    IF_DISPLAY_ENABLED(m_display.currentTemperatureChanged();)
    m_alexa.currentTemperatureChanged();
    m_thermostat.currentTemperatureChanged();
}

void MainController::wifiStatusChanged()
{
    FINE("notify wifi status changed");
    IF_DISPLAY_ENABLED(m_display.wifiStatusChanged();)
    m_alexa.wifiStatusChanged();
    m_ota.wifiStatusChanged();
}

void MainController::onSetSetting(const String& key, String& value)
{
    FINE("notify settings received");
    m_program.onSetSetting(key, value);
}

void MainController::onUpdateEvent(const UpdateEvent_t& event)
{
    FINE("notify update event");
    m_alexa.onUpdateEvent(event);
    IF_DISPLAY_ENABLED(m_display.onUpdateEvent(event);)
}


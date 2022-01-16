#include <controller.h>

MainController::MainController()
    : m_thermostat(BoilerController::Instance()),
    m_display(DisplayController::Instance()),
    m_alexa(AlexaController::Instance()),
    m_ota(OTAController::Instance()),
    m_temperature(TemperatureController::Instance()),
    m_wifi(WifiController::Instance()),
    m_program(ProgramController::Instance()),
    m_input(InputController::Instance()),
    m_state(State::Instance())
{
    m_state.addListener(this);
    m_alexa.addSettingListener(this);
}

void MainController::setup()
{
    m_wifi.connect();
    m_ota.setOnUpdateEvent([this](const UpdateEvent_t& event){onUpdateEvent(event);});
}

void MainController::handle()
{
    m_alexa.handle();
    m_ota.handle();
    m_temperature.handle();
    m_program.handle();
    m_input.handle();
    m_display.handle();
}

void MainController::targetTemperatureChanged(Cause cause)
{
    m_display.targetTemperatureChanged(cause);
    m_alexa.targetTemperatureChanged(cause);
    m_thermostat.targetTemperatureChanged();
}

void MainController::powerStateChanged(Cause cause)
{
    m_display.powerStateChanged();
    m_alexa.powerStateChanged(cause);
    m_thermostat.powerStateChanged();
}

void MainController::thermostatModeChanged(Cause cause)
{
    m_display.thermostatModeChanged(cause);
    m_alexa.thermostatModeChanged(cause);
    m_program.thermostatModeChanged();
}

void MainController::boilerStateChanged()
{
    m_display.boilerStateChanged();
}

void MainController::currentTemperatureChanged()
{
    m_display.currentTemperatureChanged();
    m_alexa.currentTemperatureChanged();
    m_thermostat.currentTemperatureChanged();
}

void MainController::wifiStatusChanged()
{
    m_display.wifiStatusChanged();
    m_alexa.wifiStatusChanged();
    m_ota.wifiStatusChanged();
}

void MainController::onSetSetting(const String& key, String& value)
{
    m_program.onSetSetting(key, value);
}

void MainController::onUpdateEvent(const UpdateEvent_t& event)
{
    m_alexa.onUpdateEvent(event);
    m_display.onUpdateEvent(event);
}


#include <controller.h>

MainController::MainController()
    : m_thermostat(BoilerController::Instance()),
    m_hwio(HWIOController::Instance()),
    m_alexa(AlexaController::Instance()),
    m_ota(OTAController::Instance()),
    m_temperature(TemperatureController::Instance()),
    m_wifi(WifiController::Instance()),
    m_program(ProgramController::Instance()),
    m_input(InputController::Instance()),
    m_state(State::Instance())
{
    m_state.addListener(this);
}

void MainController::setup()
{
    m_hwio.init();
    m_wifi.connect();
    m_ota.setOnUpdateEvent([this](const UpdateEvent_t& event){onUpdateEvent(event);});
}

void MainController::handle()
{
    m_alexa.handle();
    m_ota.handle();
    m_temperature.handle();
    m_program.handle();
    m_hwio.handle();
    m_input.handle();
}

void MainController::targetTemperatureChanged(Cause cause)
{
    m_alexa.targetTemperatureChanged(cause);
    m_thermostat.targetTemperatureChanged();
    m_hwio.targetTemperatureChanged();
}

void MainController::powerStateChanged(Cause cause)
{
    m_alexa.powerStateChanged(cause);
    m_thermostat.powerStateChanged();
    m_hwio.powerStateChanged();
}

void MainController::thermostatModeChanged(Cause cause)
{
    m_alexa.thermostatModeChanged(cause);
    m_hwio.thermostatModeChanged();
    m_program.thermostatModeChanged();
}

void MainController::boilerStateChanged()
{
    m_hwio.boilerStateChanged();
}

void MainController::currentTemperatureChanged()
{
    m_alexa.currentTemperatureChanged();
    m_thermostat.currentTemperatureChanged();
    m_hwio.currentTemperatureChanged();
}

void MainController::wifiStatusChanged()
{
    m_alexa.wifiStatusChanged();
    m_hwio.wifiStatusChanged();
    m_ota.wifiStatusChanged();
}

void MainController::onSetSetting(const String& key, String& value)
{
    m_program.onSetSetting(key, value);
}

void MainController::onUpdateEvent(const UpdateEvent_t& event)
{
    m_alexa.onUpdateEvent(event);
    m_hwio.onUpdateEvent(event);
}


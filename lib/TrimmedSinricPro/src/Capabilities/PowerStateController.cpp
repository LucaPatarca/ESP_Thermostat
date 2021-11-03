#include "PowerStateController.h"
#include "SinricPro.h"
#include "SinricProThermostat.h"

namespace SINRICPRO_NAMESPACE{

FSTR(POWERSTATE, state);             // "state"
FSTR(POWERSTATE, On);                // "On"
FSTR(POWERSTATE, Off);               // "Off"
FSTR(POWERSTATE, setPowerState);     // "setPowerState"

PowerStateController::PowerStateController() 
: event_limiter(EVENT_LIMIT_STATE) { 
  SinricProThermostat* device = static_cast<SinricProThermostat*>(this);
  device->registerRequestHandler(std::bind(&PowerStateController::handlePowerStateController, this, std::placeholders::_1));
}

/**
 * @brief Set callback function for `powerState` request
 * 
 * @param cb Function pointer to a `PowerStateCallback` function
 * @return void
 * @see PowerStateCallback
 **/
void PowerStateController::onPowerState(PowerStateCallback cb) {
  powerStateCallback = cb;
}

/**
 * @brief Send `setPowerState` event to SinricPro Server indicating actual power state
 * 
 * @param state   `true` = device turned on \n `false` = device turned off
 * @param cause   (optional) `String` reason why event is sent (default = `"PHYSICAL_INTERACTION"`)
 * @return the success of sending the even
 * @retval true   event has been sent successfully
 * @retval false  event has not been sent, maybe you sent to much events in a short distance of time
 **/
bool PowerStateController::sendPowerStateEvent(bool state, String cause) {
  if (event_limiter) return false;
  SinricProThermostat* device = static_cast<SinricProThermostat*>(this);

  DynamicJsonDocument eventMessage = device->prepareEvent(FSTR_POWERSTATE_setPowerState, cause.c_str());
  JsonObject event_value = eventMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_value];
  event_value[FSTR_POWERSTATE_state] = state ? FSTR_POWERSTATE_On : FSTR_POWERSTATE_Off;
  return device->sendEvent(eventMessage);
}

bool PowerStateController::handlePowerStateController(SinricProRequest &request) {
  SinricProThermostat* device = static_cast<SinricProThermostat*>(this);

  bool success = false;

  if (request.action == FSTR_POWERSTATE_setPowerState && powerStateCallback)  {
    bool powerState = request.request_value[FSTR_POWERSTATE_state] == FSTR_POWERSTATE_On ? true : false;
    success = powerStateCallback(device->deviceId, powerState);
    request.response_value[FSTR_POWERSTATE_state] = powerState ? FSTR_POWERSTATE_On : FSTR_POWERSTATE_Off;
    return success;
  }
  return success;
}
}
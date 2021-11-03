#include "ThermostatController.h"
#include "SinricPro.h"
#include "SinricProThermostat.h"

namespace SINRICPRO_NAMESPACE{

FSTR(THERMOSTAT, setThermostatMode);           // "setThermostatMode"
FSTR(THERMOSTAT, thermostatMode);              // "thermostatMode"
FSTR(THERMOSTAT, targetTemperature);           // "targetTemperature"
FSTR(THERMOSTAT, temperature);                 // "temperature"
FSTR(THERMOSTAT, adjustTargetTemperature);     // "adjustTargetTemperature"

ThermostatController::ThermostatController()
: event_limiter_thermostatMode(EVENT_LIMIT_STATE)
, event_limiter_targetTemperature(EVENT_LIMIT_STATE) { 
  SinricProThermostat* device = static_cast<SinricProThermostat*>(this);
  device->registerRequestHandler(std::bind(&ThermostatController::handleThermostatController, this, std::placeholders::_1)); 
}

/**
 * @brief Set callback function for `setThermostatMode` request
 * 
 * @param cb Function pointer to a `ThermostatModeCallback` function
 * @return void
 * @see ThermostatModeCallback
 **/
void ThermostatController::onThermostatMode(ThermostatModeCallback cb) {
  thermostatModeCallback = cb;
}

/**
 * @brief Set callback function for `targetTemperature` request
 * 
 * @param cb Function pointer to a `SetTargetTemperatureCallback` function
 * @return void
 * @see SetTargetTemperatureCallback
 **/
void ThermostatController::onTargetTemperature(SetTargetTemperatureCallback cb) {
  targetTemperatureCallback = cb;
}

/**
 * @brief Set callback function for `adjustTargetTemperature` request
 * 
 * @param cb Function pointer to a `AdjustTargetTemperatureCallback` function
 * @return void
 * @see AdjustTargetTemperatureCallback
 **/
void ThermostatController::onAdjustTargetTemperature(AdjustTargetTemperatureCallback cb) {
  adjustTargetTemperatureCallback = cb;
}

/**
 * @brief Send `thermostatMode` event to report a the new mode the device has been set to
 * 
 * @param   thermostatMode  String with actual mode (`AUTO`, `COOL`, `HEAT`) the device is set to
 * @param   cause           (optional) `String` reason why event is sent (default = `"PHYSICAL_INTERACTION"`)
 * @return  the success of sending the even
 * @retval  true            event has been sent successfully
 * @retval  false           event has not been sent, maybe you sent to much events in a short distance of time
 **/
bool ThermostatController::sendThermostatModeEvent(String thermostatMode, String cause) {
  if (event_limiter_thermostatMode) return false;
  SinricProThermostat* device = static_cast<SinricProThermostat*>(this);

  DynamicJsonDocument eventMessage = device->prepareEvent(FSTR_THERMOSTAT_setThermostatMode, cause.c_str());
  JsonObject event_value = eventMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_value];
  event_value[FSTR_THERMOSTAT_thermostatMode] = thermostatMode;
  return device->sendEvent(eventMessage);
}

/**
 * @brief Send `targetTemperature` event to report target temperature change
 * 
 * @param   temperature   Float with actual target temperature the device is set to
 * @param   cause         (optional) `String` reason why event is sent (default = `"PHYSICAL_INTERACTION"`)
 * @return  the success of sending the even
 * @retval  true          event has been sent successfully
 * @retval  false         event has not been sent, maybe you sent to much events in a short distance of time
 **/
bool ThermostatController::sendTargetTemperatureEvent(float temperature, String cause) {
  if (event_limiter_targetTemperature) return false;
  SinricProThermostat* device = static_cast<SinricProThermostat*>(this);

  DynamicJsonDocument eventMessage = device->prepareEvent(FSTR_THERMOSTAT_targetTemperature, cause.c_str());
  JsonObject event_value = eventMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_value];
  event_value[FSTR_THERMOSTAT_temperature] = roundf(temperature * 10) / 10.0;
  return device->sendEvent(eventMessage);
}

bool ThermostatController::handleThermostatController(SinricProRequest &request) {
  SinricProThermostat* device = static_cast<SinricProThermostat*>(this);

  bool success = false;

  if (request.action == FSTR_THERMOSTAT_targetTemperature && targetTemperatureCallback) {
    float temperature;
    if (request.request_value.containsKey(FSTR_THERMOSTAT_temperature))  {
      temperature = request.request_value[FSTR_THERMOSTAT_temperature];
    }  else {
      temperature = 1;
    }
    success = targetTemperatureCallback(device->deviceId, temperature);
    request.response_value[FSTR_THERMOSTAT_temperature] = temperature;
    return success;
  }

  if (request.action == FSTR_THERMOSTAT_adjustTargetTemperature && adjustTargetTemperatureCallback) {
    float temperatureDelta = request.request_value[FSTR_THERMOSTAT_temperature];
    success = adjustTargetTemperatureCallback(device->deviceId, temperatureDelta);
    request.response_value[FSTR_THERMOSTAT_temperature] = temperatureDelta;
    return success;
  }

  if (request.action == FSTR_THERMOSTAT_setThermostatMode && thermostatModeCallback) {
    String thermostatMode = request.request_value[FSTR_THERMOSTAT_thermostatMode] | "";
    success = thermostatModeCallback(device->deviceId, thermostatMode);
    request.response_value[FSTR_THERMOSTAT_thermostatMode] = thermostatMode;
    return success;
  }

  return success;
}
}
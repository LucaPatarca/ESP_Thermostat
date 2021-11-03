#pragma once

#include "../SinricProRequest.h"
#include "../EventLimiter.h"
#include "../SinricProStrings.h"

#include "../SinricProNamespace.h"
namespace SINRICPRO_NAMESPACE {

EXTERN_FSTR(THERMOSTAT, setThermostatMode);           // "setThermostatMode"
EXTERN_FSTR(THERMOSTAT, thermostatMode);              // "thermostatMode"
EXTERN_FSTR(THERMOSTAT, targetTemperature);           // "targetTemperature"
EXTERN_FSTR(THERMOSTAT, temperature);                 // "temperature"
EXTERN_FSTR(THERMOSTAT, adjustTargetTemperature);     // "adjustTargetTemperature"

/**
 * @brief Callback definition for onThermostatMode function
 * 
 * Gets called when device receive a `setThermostatMode` request \n
 * @param[in]   deviceId    String which contains the ID of device
 * @param[in]   mode        String with mode device should set to
 * @param[out]  mode        String device mode is set to
 * @return      the success of the request
 * @retval      true        request handled properly
 * @retval      false       request was not handled properly because of some error
 * 
 * @section ThermostatModeCallback Example-Code
 * @snippet callbacks.cpp onThermostatMode
 **/
using ThermostatModeCallback = std::function<bool(const String &, String &)>;

/**
 * @brief Callback definition for onTargetTemperature function
 * 
 * Gets called when device receive a `targetTemperature` request \n
 * @param[in]   deviceId    String which contains the ID of device
 * @param[in]   temperature Float device is requested to set the target temperature to
 * @param[out]  temperature Float target temperature temperature device is set to
 * @return      the success of the request
 * @retval      true        request handled properly
 * @retval      false       request was not handled properly because of some error
 * 
 * @section TargetTemperatureCallback Example-Code
 * @snippet callbacks.cpp onTargetTemperature
 **/
using SetTargetTemperatureCallback = std::function<bool(const String &, float &)>;

/**
 * @brief Callback definition for onAdjustTargetTemperature function
 * 
 * Gets called when device receive a `adjustTargetTemperature` reuqest \n
 * @param[in]   deviceId    String which contains the ID of device
 * @param[in]   temperature Float relative temperature device should change about
 * @param[out]  temperature Float absolute temperature device is set to
 * @return      the success of the request
 * @retval      true        request handled properly
 * @retval      false       request was not handled properly because of some error
 * 
 * @section AdjustTargetTemperatureCallback Example-Code
 * @snippet callbacks.cpp onAdjustTargetTemperature
 **/
using AdjustTargetTemperatureCallback = std::function<bool(const String &, float &)>;


/**
 * @brief ThermostatController
 * @ingroup Capabilities
 **/
class ThermostatController {
  public:
    ThermostatController();

    void onThermostatMode(ThermostatModeCallback cb);
    void onTargetTemperature(SetTargetTemperatureCallback cb);
    void onAdjustTargetTemperature(AdjustTargetTemperatureCallback cb);

    bool sendThermostatModeEvent(String thermostatMode, String cause = FSTR_SINRICPRO_PHYSICAL_INTERACTION);
    bool sendTargetTemperatureEvent(float temperature, String cause = FSTR_SINRICPRO_PHYSICAL_INTERACTION);

  protected:
    bool handleThermostatController(SinricProRequest &request);

  private:
    EventLimiter event_limiter_thermostatMode;
    EventLimiter event_limiter_targetTemperature;
    ThermostatModeCallback thermostatModeCallback;
    SetTargetTemperatureCallback targetTemperatureCallback;
    AdjustTargetTemperatureCallback adjustTargetTemperatureCallback;
};


} // SINRICPRO_NAMESPACE
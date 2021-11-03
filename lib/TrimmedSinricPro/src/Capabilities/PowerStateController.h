#pragma once

#include "../SinricProRequest.h"
#include "../EventLimiter.h"
#include "../SinricProStrings.h"

#include "../SinricProNamespace.h"
namespace SINRICPRO_NAMESPACE {

EXTERN_FSTR(POWERSTATE, state);             // "state"
EXTERN_FSTR(POWERSTATE, On);                // "On"
EXTERN_FSTR(POWERSTATE, Off);               // "Off"
EXTERN_FSTR(POWERSTATE, setPowerState);     // "setPowerState"

/**
 * @brief Callback definition for onPowerState function
 * 
 * Gets called when device receive a `setPowerState` reuqest \n
 * @param[in]   deviceId    String which contains the ID of device
 * @param[in]   state       `true` = device is requested to turn on \n `false` = device is requested to turn off
 * @param[out]  state       `true` = device has been turned on \n `false` = device has been turned off
 * @return      the success of the request
 * @retval      true        request handled properly
 * @retval      false       request was not handled properly because of some error
 * @section PowerStateCallback Example-Code
 * @snippet callbacks.cpp onPowerState
 **/
using PowerStateCallback = std::function<bool(const String &, bool &)>;


/**
 * @brief PowerStateController
 * @ingroup Capabilities
 **/
class PowerStateController {
  public:
    PowerStateController();

    void onPowerState(PowerStateCallback cb);
    bool sendPowerStateEvent(bool state, String cause = FSTR_SINRICPRO_PHYSICAL_INTERACTION);

  protected:
    bool handlePowerStateController(SinricProRequest &request);

  private:
    EventLimiter event_limiter;
    PowerStateCallback powerStateCallback;
};


} // SINRICPRO_NAMESPACE
#pragma once

#include "../EventLimiter.h"
#include "../SinricProStrings.h"

#include "../SinricProNamespace.h"
namespace SINRICPRO_NAMESPACE {

EXTERN_FSTR(TEMPERATURE, currentTemperature);    // "currentTemperature"
EXTERN_FSTR(TEMPERATURE, humidity);              // "humidity"
EXTERN_FSTR(TEMPERATURE, temperature);           // "temperature"

/**
 * @brief TemperatureSensor
 * @ingroup Capabilities
 **/
class TemperatureSensor {
  public:
    TemperatureSensor();
    bool sendTemperatureEvent(float temperature, float humidity = -1, String cause = FSTR_SINRICPRO_PERIODIC_POLL);
  private:
    EventLimiter event_limiter;
};


} // SINRICPRO_NAMESPACE
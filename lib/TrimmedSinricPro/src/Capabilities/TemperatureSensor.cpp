#include "TemperatureSensor.h"
#include <ArduinoJson.h>
#include "SinricPro.h"
#include "SinricProThermostat.h"

namespace SINRICPRO_NAMESPACE{

FSTR(TEMPERATURE, currentTemperature);    // "currentTemperature"
FSTR(TEMPERATURE, humidity);              // "humidity"
FSTR(TEMPERATURE, temperature);           // "temperature"

TemperatureSensor::TemperatureSensor() 
: event_limiter(EVENT_LIMIT_SENSOR_VALUE) {}

/**
 * @brief Send `currentTemperature` event to report actual temperature (measured by a sensor)
 * 
 * @param   temperature   `float` actual temperature measured by a sensor
 * @param   humidity      `float` (optional) actual humidity measured by a sensor (default=-1.0f means not supported)
 * @param   cause         (optional) `String` reason why event is sent (default = `"PERIODIC_POLL"`)
 * @return  the success of sending the even
 * @retval  true          event has been sent successfully
 * @retval  false         event has not been sent, maybe you sent to much events in a short distance of time
 **/
bool TemperatureSensor::sendTemperatureEvent(float temperature, float humidity, String cause) {
  if (event_limiter) return false;
  SinricProThermostat* device = static_cast<SinricProThermostat*>(this);

  DynamicJsonDocument eventMessage = device->prepareEvent(FSTR_TEMPERATURE_currentTemperature, cause.c_str());
  JsonObject event_value = eventMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_value];
  event_value[FSTR_TEMPERATURE_humidity] = roundf(humidity * 100) / 100.0;
  event_value[FSTR_TEMPERATURE_temperature] = roundf(temperature * 10) / 10.0;
  return device->sendEvent(eventMessage);
}
}
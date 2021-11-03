/*
 *  Copyright (c) 2019 Sinric. All rights reserved.
 *  Licensed under Creative Commons Attribution-Share Alike (CC BY-SA)
 *
 *  This file is part of the Sinric Pro (https://github.com/sinricpro/)
 */

#pragma once

#include "SinricProDevice.h"
#include "Capabilities/SettingController.h"
#include "Capabilities/PowerStateController.h"
#include "Capabilities/ThermostatController.h"
#include "Capabilities/TemperatureSensor.h"

#include "SinricProNamespace.h"
namespace SINRICPRO_NAMESPACE {

/**
 * @class SinricProThermostat
 * @brief Device to control Thermostat
 * @ingroup Devices
 * 
 * Support
 * * Set / adjust target temperature
 * * Report target temperature
 * * Report actual temperature
 * * Set thermostat mode `AUTO`, `COOL`, `HEAT`
 **/
class SinricProThermostat :  public SinricProDevice,
                             public SettingController,
                             public PowerStateController,
                             public ThermostatController,
                             public TemperatureSensor {
                             friend class SettingController;
                             friend class PowerStateController;
                             friend class ThermostatController;
                             friend class TemperatureSensor;
  public:
	  SinricProThermostat(const String &deviceId) : SinricProDevice(deviceId, "THERMOSTAT") {}
};

} // SINRICPRO_NAMESPACE
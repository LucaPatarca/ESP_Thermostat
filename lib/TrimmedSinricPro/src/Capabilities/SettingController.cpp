#include "SettingController.h"
#include "SinricProThermostat.h"
#include "SinricPro.h"

namespace SINRICPRO_NAMESPACE{

FSTR(SETTING, setSetting);     // "setSetting"
FSTR(SETTING, id);             // "id"
FSTR(SETTING, value);          // "value"

SettingController::SettingController() { 
  SinricProThermostat* device = static_cast<SinricProThermostat*>(this);
  device->registerRequestHandler(std::bind(&SettingController::handleSettingController, this, std::placeholders::_1)); 
}

void SettingController::onSetSetting(SetSettingCallback cb) {
  setSettingCallback = cb;
}

bool SettingController::handleSettingController(SinricProRequest &request) {
  SinricProThermostat* device = static_cast<SinricProThermostat*>(this);

  bool success = false;

  if (setSettingCallback && request.action == FSTR_SETTING_setSetting) {
    String settingId    = request.request_value[FSTR_SETTING_id] | "";
    String settingValue = request.request_value[FSTR_SETTING_value] | "";
    success = setSettingCallback(device->deviceId, settingId, settingValue);
    request.response_value[FSTR_SETTING_id]    = settingId;
    request.response_value[FSTR_SETTING_value] = settingValue;
    return success;
  }

  return success;
}
}
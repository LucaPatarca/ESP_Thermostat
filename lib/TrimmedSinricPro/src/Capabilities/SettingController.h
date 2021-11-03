#pragma once

#include "../SinricProRequest.h"
#include "../SinricProStrings.h"

#include "../SinricProNamespace.h"
namespace SINRICPRO_NAMESPACE {

using SetSettingCallback = std::function<bool(const String&, const String&, String&)>;

EXTERN_FSTR(SETTING, setSetting);     // "setSetting"
EXTERN_FSTR(SETTING, id);             // "id"
EXTERN_FSTR(SETTING, value);          // "value"

class SettingController {
  public:
    SettingController();
    void onSetSetting(SetSettingCallback cb);

  protected:
    bool handleSettingController(SinricProRequest &request);

  private: 
    SetSettingCallback setSettingCallback;
};


} // SINRICPRO_NAMESPACE
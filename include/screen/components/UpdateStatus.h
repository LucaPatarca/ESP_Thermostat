#pragma once

#include <screen/ScreenElement.h>
#include <UpdateListener.h>

class UpdateStatus: public ScreenElement<UpdateEvent_t>{
    using ScreenElement<UpdateEvent_t>::ScreenElement;
    void drawImpl() override;
};
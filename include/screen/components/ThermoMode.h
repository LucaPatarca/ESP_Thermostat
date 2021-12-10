#pragma once

#include <screen/ScreenElement.h>
#include <StateListener.h>

class ThermoMode: public ScreenElement<Mode>{
    using ScreenElement<Mode>::ScreenElement;
    void drawImpl() override;
};
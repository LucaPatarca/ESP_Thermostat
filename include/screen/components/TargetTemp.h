#pragma once

#include <screen/ScreenElement.h>

class TargetTemp: public ScreenElement<float>{
    using ScreenElement<float>::ScreenElement;
    void drawImpl() override;
};
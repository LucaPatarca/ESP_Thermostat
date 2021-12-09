#pragma once

#include <screen/ScreenElement.h>

class CurrentTemp: public ScreenElement<float>{
    using ScreenElement<float>::ScreenElement;
    void drawImpl() override;
};
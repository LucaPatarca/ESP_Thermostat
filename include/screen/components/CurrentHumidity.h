#pragma once

#include <screen/ScreenElement.h>

class CurrentHumidity: public ScreenElement<float>{
    using ScreenElement<float>::ScreenElement;
    void drawImpl() override;
};
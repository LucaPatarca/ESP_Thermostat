#pragma once

#include <screen/ScreenElement.h>

class BoilerState: public ScreenElement<bool>{
    using ScreenElement<bool>::ScreenElement;
    void drawImpl() override;
};
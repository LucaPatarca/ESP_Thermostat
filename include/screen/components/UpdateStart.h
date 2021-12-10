#pragma once

#include <screen/ScreenElement.h>

class UpdateStart: public ScreenElement<bool>{
    using ScreenElement<bool>::ScreenElement;
    void drawImpl() override;
};
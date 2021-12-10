#pragma once

#include <screen/ScreenElement.h>

class UpdateEnd: public ScreenElement<bool>{
    using ScreenElement<bool>::ScreenElement;
    void drawImpl() override;
};
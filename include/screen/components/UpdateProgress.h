#pragma once

#include <screen/ScreenElement.h>

class UpdateProgress: public ScreenElement<int>{
    using ScreenElement<int>::ScreenElement;
    void drawImpl() override;
};
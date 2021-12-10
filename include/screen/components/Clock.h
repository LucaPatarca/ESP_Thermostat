#pragma once

#include <screen/ScreenElement.h>
#include <ntime.h>

class Clock: public ScreenElement<Time_t>{
    using ScreenElement<Time_t>::ScreenElement;
    void drawImpl() override;
};
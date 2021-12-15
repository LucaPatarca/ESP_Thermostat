#pragma once

#include <screen/AnimatedScreenElement.h>
#include <ntime.h>

#define DOTS_VISIBLE 0
#define DOTS_NOT_VISIBLE 1

class Clock: public AnimatedScreenElement<Time_t>{
    using AnimatedScreenElement<Time_t>::AnimatedScreenElement;
    void drawImpl() override;
    void tickImpl() override;
};
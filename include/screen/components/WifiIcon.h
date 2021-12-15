#pragma once

#include <WiFiListener.h>
#include <screen/AnimatedScreenElement.h>

#define BAR1 0
#define BAR2 1
#define BAR3 2

class WifiIcon : public AnimatedScreenElement<WiFiStatus>{
public:
    using AnimatedScreenElement<WiFiStatus>::AnimatedScreenElement;
    void drawImpl() override;
    void tickImpl() override;
};
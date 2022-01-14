#pragma once

#include<screen/screen.h>

class BoilerState: public ScreenElement{
    using ScreenElement::ScreenElement;
    void drawImpl() override;
};

class Clock: public AnimatedScreenElement{
    using AnimatedScreenElement::AnimatedScreenElement;
    void drawImpl() override;
    void tickImpl() override;
};

class CurrentHumidity: public ScreenElement{
    using ScreenElement::ScreenElement;
    void drawImpl() override;
};

class CurrentTemp: public ScreenElement{
    using ScreenElement::ScreenElement;
    void drawImpl() override;
};

class TargetTemp: public ScreenElement{
    using ScreenElement::ScreenElement;
    void drawImpl() override;
};

class TempTrend: public ScreenElement{
    using ScreenElement::ScreenElement;
    void drawImpl() override;
};

class ThermoMode: public ScreenElement{
    using ScreenElement::ScreenElement;
    void drawImpl() override;
};

class WifiIcon : public AnimatedScreenElement{
    using AnimatedScreenElement::AnimatedScreenElement;
    void drawImpl() override;
    void tickImpl() override;
};
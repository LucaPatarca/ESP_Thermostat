#pragma once

#include <screen/ScreenElement.h>

#include <TemperatureListener.h>

class TempTrend: public ScreenElement<TemperatureTrend>{
    using ScreenElement<TemperatureTrend>::ScreenElement;
    void drawImpl() override;
};
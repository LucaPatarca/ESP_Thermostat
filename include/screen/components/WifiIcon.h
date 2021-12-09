#pragma once

#include <WiFiListener.h>
#include <Adafruit_SSD1306.h>
#include <screen/ScreenElement.h>

class WifiIcon : public ScreenElement<WiFiStatus>{
public:
    using ScreenElement<WiFiStatus>::ScreenElement;
    void drawImpl() override;
};
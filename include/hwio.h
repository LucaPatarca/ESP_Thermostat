#pragma once

#include <Adafruit_SSD1306.h>
#include <temperature.h>

#define HWIO_DEBUG

#ifdef ENABLE_DEBUG
#define HWIO_DEBUG
#endif

class HWIOController{
    private:
        Adafruit_SSD1306 *m_display;
        bool m_powerState;
        bool m_boilerState;
        Temperature m_temp;
        unsigned long m_nextUpdate;
        bool m_needsUpdate;

        void setDisplay(int size);
    public:
        HWIOController();
        
        void setPowerState(bool state);
        void setBoilerState(bool state);
        void setTemperature(Temperature temp);
        void update();

        void wifiConnecting();
        void wifiConnected();

        void updateProgress(float progress);
        void updateComplete();
};
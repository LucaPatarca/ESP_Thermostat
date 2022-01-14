#include <screen/screen.h>
#include <listeners.h>

class UpdateScreen: public Screen{
public:
    UpdateScreen(Adafruit_SSD1306 *display);

    void onUpdateEvent(UpdateEvent_t&);

    void draw() override;
    void refresh() override;
};
#include <UpdateListener.h>
#include <screen/Screen.h>
#include <screen/ScreenElement.h>

class UpdateScreen: public Screen, public UpdateListener{
private:
    ScreenElement<bool> *_updateStart;
    ScreenElement<bool> *_updateEnd;
    ScreenElement<int> *_updateProgress;
public:
    UpdateScreen(Adafruit_SSD1306 *display);

    void onUpdateEvent(UpdateEvent_t event) override;

    void draw() override;
    void refresh() override;
};
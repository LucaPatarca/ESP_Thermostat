#include <screen/UpdateScreen.h>
#include <screen/components/UpdateProgress.h>
#include <screen/components/UpdateStart.h>
#include <screen/components/UpdateEnd.h>

UpdateScreen::UpdateScreen(Adafruit_SSD1306 *display):Screen(display){
    _updateProgress = new UpdateProgress(_display);
    _updateStart = new UpdateStart(_display);
    _updateEnd = new UpdateEnd(_display);
    _updateStart->setStatus(false);
    _updateEnd->setStatus(false);
}

void UpdateScreen::onUpdateEvent(UpdateEvent_t event){
    switch (event.type)
    {
    case UpdateEventType::START:
        _updateStart->setStatus(true);
        break;
    case UpdateEventType::PROGRESS:
        _updateProgress->setStatus((int)event.progress);
        break;
    case UpdateEventType::END:
        _updateEnd->setStatus(true);
        break;

    default:
        break;
    }
}

void UpdateScreen::draw(){
    _updateStart->draw();
    _updateProgress->draw();
    _updateEnd->draw();
}

void UpdateScreen::refresh(){
    _updateProgress->refresh();
    _updateStart->refresh();
    _updateStart->refresh();
}
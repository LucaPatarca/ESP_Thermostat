#include <EventEmitter.h>
#include <BoilerListener.h>
#include <StateListener.h>
#include <WiFiListener.h>
#include <TemperatureListener.h>
#include <UpdateListener.h>

template<typename Listener>
void EventEmitter<Listener>::addListener(Listener *listener){
    _listeners.push_back(listener);
}

template<typename Listener>
void EventEmitter<Listener>::removeListener(Listener *listener){
    _listeners.remove(listener);
}

template class EventEmitter<BoilerListener>;
template class EventEmitter<StateListener>;
template class EventEmitter<WiFiListener>;
template class EventEmitter<TemperatureListener>;
template class EventEmitter<UpdateListener>;
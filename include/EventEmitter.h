#pragma once

#include <list>

template<typename Listener>
class EventEmitter{
    protected:
        std::list<Listener*> _listeners;
    public:
        void addListener(Listener *listener);
        void removeListener(Listener *listener);
};
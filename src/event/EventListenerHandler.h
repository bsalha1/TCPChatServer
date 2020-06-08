#pragma once

#include <iostream>
#include <string>
#include <list>

#include "EventListener.h"

using namespace std;

class EventListenerHandler
{
    private:
        list<EventListener*> listeners;

    public:
        void registerEventListener(EventListener* listener);

        void unregisterEventListener(EventListener* listener);

        void onEventPing(EventPing* event);

        void onEventQuit(EventQuit* event);
};
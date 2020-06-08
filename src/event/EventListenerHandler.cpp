#include <iostream>

#include "EventListenerHandler.h"

using namespace std;

void EventListenerHandler::registerEventListener(EventListener* listener)
{
    listeners.push_back(listener);
}

void EventListenerHandler::unregisterEventListener(EventListener* listener)
{
    listeners.remove(listener);
}

void EventListenerHandler::onEventPing(EventPing* event)
{
    for(EventListener* listener : listeners)
    {
        cout << "Dispatching EventPing" << endl;
        listener->onEventPing((EventPing*)event);
    }
}

void EventListenerHandler::onEventQuit(EventQuit* event)
{
    for(EventListener* listener : listeners)
    {
        cout << "Dispatching EventQuit" << endl;
        listener->onEventQuit((EventQuit*)event);
    }
}
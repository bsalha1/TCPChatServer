#pragma once

#include <iostream>
#include <string>

#include "EventPing.h"
#include "EventQuit.h"

using namespace std;

class EventListener
{
    public:
        virtual void onEventPing(EventPing* event);
        virtual void onEventQuit(EventQuit* event);
};
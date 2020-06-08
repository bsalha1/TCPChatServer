#include <iostream>
#include <string>
#include <mutex>

#include "QuitListener.h"

using namespace std;

void QuitListener::onEventQuit(EventQuit* event)
{
    lock.unlock();
}

void QuitListener::waitForCall()
{
    lock.lock();
}
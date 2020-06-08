#include <mutex>

#include "EventListener.h"

using namespace std;

class QuitListener : public EventListener
{
    private:
        mutex lock;

    public:
        void onEventQuit(EventQuit* event) override;

        void waitForCall();
};
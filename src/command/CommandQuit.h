#pragma once

#include <string>
#include <list>

#include "../network/TCPChatClient.h"
#include "CommandAbstract.h"

using namespace std;

class CommandQuit : public CommandAbstract
{
    private:
        TCPChatClient* client;
        int socket;
        
    public:
        CommandQuit(TCPChatClient* client, int socket);

        void execute(list<string> args) override;
};
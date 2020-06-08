#include "CommandQuit.h"
#include "../request/RequestQuit.h"
#include "../event/QuitListener.h"

using namespace std;

CommandQuit::CommandQuit(TCPChatClient* client, int socket) : CommandAbstract("quit", "leave the server")
{
    this->client = client;
    this->socket = socket;
}

void CommandQuit::execute(list<string> args)
{
    // Send request for PingEvent
    RequestQuit* request = new RequestQuit();
    request->sendRequest(client, socket);
    client->~TCPClient();
}
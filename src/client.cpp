#include <iostream>
#include <thread>
#include <string>
#include <list>
#include <iterator>
#include <mutex>
#include <exception>
#include <chrono>

#include <signal.h>

#include <sys/socket.h>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "network/TCPChatClient.h"
#include "command/CommandPing.h"
#include "command/CommandHelp.h"
#include "command/CommandQuit.h"

void exitHandler(int signal);
int main(int argc, char ** argv)
{
    if(argc != 3)
    {
        fprintf(stderr, "[FAIL] Must enter two arguments; address and port.\n");
        return EXIT_FAILURE;
    }
    signal(SIGINT, exitHandler);

    char * address = argv[1];
    short port = (short) atoi(argv[2]);

    TCPChatClient* client = new TCPChatClient(address, port);
    client->registerCommand(new CommandHelp(client->getCommandHandler()));
    client->registerCommand(new CommandQuit(client, client->getLocalSocket()));
    client->registerCommand(new CommandPing(client, client->getLocalSocket()));
    client->wait();

    exit(EXIT_SUCCESS);
}

void exitHandler(int signal)
{
    exit(EXIT_SUCCESS);
}
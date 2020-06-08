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

#include "TCPChatClient.h"
#include "max.h"

#include "../event/EventListenerHandler.h"
#include "../event/PingListener.h"

#include "../command/CommandHandler.h"
#include "../command/CommandAbstract.h"

using namespace std;


TCPChatClient::TCPChatClient(char* address, unsigned short port) : TCPClient(address, port)
{
    eventHandler = new EventListenerHandler();
    commandHandler = new CommandHandler();
    
    localSocket = makeSocket();
    try
    {
        connectToServer(localSocket);
    }
    catch(exception& e)
    {
        fprintf(stdout, "%s", e.what());
    }

    isRunning = true;
    getInput = new thread(&TCPChatClient::getInputThread, this);
    getServerData = new thread(&TCPChatClient::getServerDataThread, this);
    printf("TCPChatClient started on %s:%d\n", address, port);
}

/*
    Get client input then send to server
*/
void TCPChatClient::getInputThread()
{
    #ifdef ERR_MSG
    fprintf(stderr, "[OK] Input thread initialized\n");
    #endif
    char message[max_message];
    while(isRunning)
    {
        read(STDIN_FILENO, message, sizeof(message)); // Thread pauses until input is read
        message[strcspn(message, "\n")] = 0; // Cut off \n

        // Handle Command
        if(message[0] == '/')
        {
            // Cut off '/'
            memmove(message, message + 1, strlen(message));
            commandHandler->handleCommand(message);
            continue;
        }

        try
        {
            sendData(message, strlen(message), localSocket);
        }
        catch(exception &e)
        {
            stdoutMut.lock();
            fprintf(stdout, "%s", e.what());
            stdoutMut.unlock();
            break;
        }
    }
}

/*
    Get data of server
*/
void TCPChatClient::getServerDataThread()
{
    #ifdef ERR_MSG
    fprintf(stderr, "[OK] Server data thread initialized\n");
    #endif
    char buffer[max_message + max_username + 2];
    ssize_t sizeData;
    while(isRunning)
    {
        try
        {
            sizeData = receiveData(buffer, sizeof(buffer), localSocket); // Client pauses here
            buffer[sizeData] = '\0';
        }
        catch(exception& e)
        {
            stdoutMut.lock();
            fprintf(stdout, "%s", e.what());
            stdoutMut.unlock();
            break;
        }

        // If event received
        if((strlen(buffer) > 0) && (buffer[0] == '/'))
        {
            printf("Received Event: %s\n", buffer);
            if(strcmp(buffer, "/event/Ping") == 0)
            {
                eventHandler->onEventPing(new EventPing(chrono::high_resolution_clock::now()));
            }
            if(strcmp(buffer, "/event/Quit") == 0)
            {
                eventHandler->onEventQuit(new EventQuit());
                break;
            }
            continue;
        }
        
        // Server close handshake
        // if(strcmp(buffer, SERVER_QUIT) == 0) break;
        fprintf(stdout, "%s\n", buffer);
    }
    return;
}

EventListenerHandler* TCPChatClient::getEventListenerHandler()
{
    return this->eventHandler;
}

CommandHandler* TCPChatClient::getCommandHandler()
{
    return this->commandHandler;
}

int TCPChatClient::getLocalSocket()
{
    return this->localSocket;
}

void TCPChatClient::registerCommand(CommandAbstract* command)
{
    this->commandHandler->registerCommand(command);
}

void TCPChatClient::registerEventListener(EventListener* listener)
{
    this->eventHandler->registerEventListener(listener);
}

void TCPChatClient::unregisterEventListener(EventListener* listener)
{
    this->eventHandler->unregisterEventListener(listener);
}

void TCPChatClient::wait()
{
    // Wait for server to close
    getServerData->join();
    // sendData((void *) CLIENT_QUIT, strlen(CLIENT_QUIT), localSocket);
    mut.lock();
    try
    {
        closeSocket(localSocket);
    }
    catch(exception& e)
    {
        fprintf(stderr, "%s", e.what());
    }
    mut.unlock();

    stdoutMut.lock();
    fprintf(stdout, "Server closed, press enter to exit.\n");
    stdoutMut.unlock();

    isRunning = false;
    getInput->join();
}
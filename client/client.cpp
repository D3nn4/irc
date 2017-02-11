#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "client.hpp"

Client::Client(int socket)
    :_socket(socket){}

Client::~Client()
{
    close(_socket);
}

int Client::getSocket() const
{
    return _socket;
}


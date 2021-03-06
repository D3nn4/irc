#include <string>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "client.hpp"

Client::Client(int socket, std::string pseudo)
    :_socket(socket),
     _pseudo(pseudo){}

Client::Client(int socket)
    :_socket(socket){}

Client::Client(const Client && src)
{
    _socket = src._socket;
    _pseudo = src._pseudo;

}
Client::~Client()
{
    std::cout << "destructor Client\n";
    close(_socket);
}

int Client::getSocket() const
{
    return _socket;
}

std::string Client::getPseudo() const
{
    return _pseudo;
}

void Client::setPseudo(std::string pseudo)
{
    std::cout << "In set pseudo\n";
    _pseudo = pseudo;
}


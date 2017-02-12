#ifndef __SERVER_HPP__
#define __SERVER_HPP__
#include <string>
#include <stdbool.h>
#include <errno.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <set>
#include <sys/time.h>
#include <sys/select.h>
#include <functional>
#include <map>
#include "client.hpp"

class Server
{
public:

    Server(char *port);
		void getFds();
    bool actionsOnFds();
    void addNewClient();
    void readFd(Client& read_sock);
    void writeFd(Client& usr, char* buffer);
    

    typedef std::map<int, Client> ClientList; 
    int sockfd;
    int portno;
    struct sockaddr_in serv_addr;
    ClientList clients;
    struct timeval timeout;
    fd_set readfds;
};

#endif // __SERVER_HPP__

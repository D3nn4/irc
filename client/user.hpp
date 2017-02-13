#ifndef __USER_HPP__
#define __USER_HPP__
#include <string>
#include <stdbool.h>
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

class User
{
public:

    User(std::string ip, std::string port, std::string pseudo);
		void getFds();
    bool actionsOnFds();
    void addNewClient();
    bool readFd();
    bool writeFd();

    typedef std::map<int, Client> ClientList; 
    int sockfd;
    std::string ip_Server;
    int portno;
    struct sockaddr_in serv_addr;
    ClientList clients;
    struct timeval timeout;
    fd_set readfds;
};

#endif // __USER_HPP__

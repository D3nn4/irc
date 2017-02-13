#include <stdio.h>
#include <stdbool.h>
#include <string>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/select.h>
#include <iostream>
#include "user.hpp"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

User::User(std::string ip, std::string port, std::string pseudo)
    :ip_Server(ip)
{
    // std::cout << "In constructor\n";
    portno = atoi(port.c_str());
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    int ret = inet_pton(AF_INET, ip.c_str(), &(serv_addr.sin_addr));	
    if (ret == -1)
        error("Error, no such host\n");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    int n = write(sockfd,pseudo.c_str(),(int)pseudo.size());
    if (n < 0) 
        error("ERROR writing to socket");
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
}

void User::getFds()
{
    // std::cout << "In getFds\n";
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    FD_SET(sockfd, &readfds);
}

bool User::writeFd()
{
    // std::cout << "In writeFd\n";
    char buffer[256];
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    std::string str = buffer;
    if(str.compare("q\n") == 0)
        return false;
    int n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
        error("ERROR writing to socket");
    return true;
}

bool User::readFd()
{
    // std::cout << "In readFd\n";
    char buffer[256];
    bzero(buffer,256);
    int n = read(sockfd,buffer,255);
    if (n < 0) 
        error("ERROR reading from socket");
    if(n == 0) {
        std::cout << "Server disconnected, disconnection.\n";
        return false;
    }
    std::cout << buffer << "\n";
    return true;
}

bool User::actionsOnFds()
{
    // std::cout << "In actionsOnFds\n";
    int max_fd = STDIN_FILENO > sockfd ? STDIN_FILENO : sockfd;
    getFds();
    int retval = select(max_fd + 1, &readfds, NULL, NULL, &timeout);
    if (retval < 0)
        error("error select");
    if (FD_ISSET(STDIN_FILENO, &readfds))
        return writeFd();
    else if (FD_ISSET(sockfd, &readfds))
        return readFd();
    else
        return true;
}

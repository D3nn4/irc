#include <string>
#include <map>
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
#include <iostream>
#include "client.hpp"
#include "server.hpp"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

// static bool compFun(const Client& lhs, const Client& rhs)
// {
//     return lhs.getSocket() < rhs.getSocket();
// }

Server::Server(char *port)
{
    // std::cout << "In constructor Server\n";
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(port);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    if (listen(sockfd,5) == -1)
        error("ERROR on listen");
    // std::cout << "before emplace \n";
    auto ret = clients.emplace(sockfd, sockfd);
    std::string pseudo = "server";
    ret.first->second.setPseudo(pseudo);
    // std::cout << "after emplace \n";
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

}

void Server::getFds()
{
    // std::cout << "In getFds\n";
    FD_ZERO(&readfds);
    Server::ClientList::iterator it = clients.begin();
    if (!clients.empty()){
        for (; it != clients.end(); ++it) {
            FD_SET((it->second).getSocket(), &readfds);
        }
    }
}

void Server::writeFd(Client& usr,std::string buffer)
{
    // std::cout << "in writefd\n";
    int fd = usr.getSocket();
    ClientList::iterator it_current = clients.begin();
    for (; it_current != clients.end(); ++it_current) {
        int write_fd = (it_current->second).getSocket();
        if (fd == write_fd && (usr.getPseudo()).empty())
            usr.setPseudo(buffer);
        if (fd != write_fd && write_fd != sockfd) {
            if (!(usr.getPseudo()).empty()) {
                std::string msg = usr.getPseudo();
                msg += ": ";
                msg += buffer;
                int n = write(write_fd, msg.c_str(), (int)msg.size() - 1);
                if (n < 0)
                    error("error writing msg");
            }
            else {
                usr.setPseudo(buffer);
            }
        }
    }
}

void Server::addNewClient()
{
    // std::cout << "In addNewClient\n";
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    int newsockfd = 0;
    if ((newsockfd = accept(sockfd,
                            (struct sockaddr *) &cli_addr,
                            &clilen)) < 0) {
        error("ERROR on accept");
    }
    clients.emplace(newsockfd,newsockfd);
    write(newsockfd, "welcome", strlen("welcome"));
    FD_CLR(sockfd, &readfds);
}

void Server::readFd(Client& read_sock)
{
    // std::cout << "in readFd\n";
    char buffer[256];
    bzero(buffer,256);
    int fd = read_sock.getSocket();
    int n = read(fd,buffer,255);
    if (n < 0)
        error("ERROR reading from socket");
    if (n == 0){
        FD_CLR(fd, &readfds);
        clients.erase(read_sock.getSocket());
    }
    else {
        std::string str = buffer;
        std::cout << "Here is the message:" << str << "\n";
        writeFd(read_sock, str);
    }
}

bool Server::actionsOnFds(){
    // std::cout << "In actionsOnFds\n";
    int max_fd = clients.rbegin()->first;
    FD_ZERO(&readfds);
    getFds();
    int retval = select(max_fd + 1, &readfds, NULL, NULL, &timeout);
    if (retval < 0)
        error("Error select");
    if(FD_ISSET(sockfd, &readfds))
        addNewClient();
    ClientList::iterator it_list = clients.begin();
    for (; it_list != clients.end(); ++it_list) {
        Client& read_sock = it_list->second;
        if (FD_ISSET(read_sock.getSocket(), &readfds)
            && read_sock.getSocket() != sockfd)
            readFd(read_sock);
    }
    return true;
}

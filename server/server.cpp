#include <string>
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
// struct Client {
// public:
//     Client(void) = delete;
//     Client(int sockfd) : _sockfd(sockfd){};

//     int _sockfd;

//     int getSocket(void) const { return this->_sockfd; };
// };

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void getFds(std::set<Client, bool(*)(const Client&, const Client&)> &listClient, fd_set &readfds) {
    //std::cout << "in getFds\n";
    FD_ZERO(&readfds);
    std::set<Client>::iterator it = listClient.begin();
    if (!listClient.empty()){
        for (; it != listClient.end(); ++it) {
            FD_SET((*it).getSocket(), &readfds);
        }
    }
}
int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n, retval;
     auto compFun = [] (const Client& c1, const Client& c2) -> bool
                    {
                        return c1.getSocket() < c2.getSocket();
                    };
     std::set<Client, bool(*)(const Client&, const Client&)> listClient(compFun);
     fd_set readfds;
     struct timeval timeout;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     if (listen(sockfd,5) == -1)
         error("ERROR on listen");
     clilen = sizeof(cli_addr);
     listClient.emplace(sockfd);
     std::cout << sockfd << "\n";
     
     while(true) {
         FD_ZERO(&readfds);
         getFds(listClient, readfds);
         timeout.tv_sec = 1;
         timeout.tv_usec = 0;
         int max_fd = (*(listClient.rbegin())).getSocket() + 1;
         retval = select(max_fd, &readfds, NULL, NULL, &timeout);
         std::cout << "retval " << retval << "\n";
         if (retval < 0)
             error("Error select");
         if (FD_ISSET(sockfd, &readfds)) {
             std::cout << "lets accept\n";
             if ((newsockfd = accept(sockfd,
                                    (struct sockaddr *) &cli_addr,
                                     &clilen)) < 0) {
                 error("ERROR on accept");
             }
             // Client newClient(newsockfd);
             listClient.emplace(newsockfd);
             write(newsockfd, "welcome", strlen("welcome"));
             FD_CLR(sockfd, &readfds);

         }
         std::set<Client, bool(*)(const Client&, const Client&)>::iterator it_list = listClient.begin();
         for (; it_list != listClient.end(); ++it_list) {
             int curr_sock = (*it_list).getSocket();
             if (FD_ISSET(curr_sock, &readfds)) {
                 bzero(buffer,256);
                 n = read(curr_sock,buffer,255);
                 if (n < 0)
                     error("ERROR reading from socket");
                 if (n == 0){
                     FD_CLR(curr_sock, &readfds);
                     listClient.erase(curr_sock);
                 }
                 else {
                     std::string str = buffer;
                     printf("Here is the message: %s\n",buffer);
                 }
                 std::set<Client, bool(*)(const Client&, const Client&)>::iterator it_current = listClient.begin();
                 for (; it_current != listClient.end(); ++it_current) {
                     int current_fd = (*it_current).getSocket();
                     if (curr_sock != current_fd && current_fd != sockfd)
                         n = write(current_fd, buffer,strlen(buffer));
                     if (n < 0)
                         error("ERROR writing to socket");
                 }
             }
         }
     }
     close(newsockfd);
     close(sockfd);
     return 0;
}

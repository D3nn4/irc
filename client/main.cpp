#include <stdio.h>
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

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n, retval, max_fd;
    struct sockaddr_in serv_addr;
    char buffer[256];
    fd_set readfds;
    struct timeval timeout;
    
    if (argc < 4) {
        std::cout << "Not enought arguments.\n";
        exit(0);
    }
    std::string pseudo = argv[3];
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    int ret = 0;
    ret = inet_pton(AF_INET, argv[1], &(serv_addr.sin_addr));	
    if (ret == -1) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    n = write(sockfd,pseudo.c_str(),(int)pseudo.size());
    if (n < 0) 
        error("ERROR writing to socket");
    while (true)
        {
            FD_ZERO(&readfds);
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;

            bzero(buffer,256);
            FD_SET(STDIN_FILENO, &readfds);
            FD_SET(sockfd, &readfds);
            max_fd = STDIN_FILENO > sockfd ? STDIN_FILENO : sockfd;
            retval = select(max_fd + 1, &readfds, NULL, NULL, &timeout);
            if (retval < 0)
                error("error select");
            if (FD_ISSET(STDIN_FILENO, &readfds)) {
                fgets(buffer,255,stdin);
                std::string str = buffer;
                if(str.compare("q\n") == 0)
                    break;
                n = write(sockfd,buffer,strlen(buffer));
                if (n < 0) 
                    error("ERROR writing to socket");
            }
            if (FD_ISSET(sockfd, &readfds)) {
                bzero(buffer,256);
                n = read(sockfd,buffer,255);
                if (n < 0) 
                    error("ERROR reading from socket");
                if(n == 0) {
                    std::cout << "Server disconnected, disconnection.\n";
                    break;
                }
                std::cout << buffer << "\n";
            }
        }
    close(sockfd);
    return 0;
}

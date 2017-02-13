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
#include <stdbool.h>
#include "user.hpp"

int main(int argc, char *argv[])
{
    if (argc < 4) {
        std::cout << "Not enought arguments.\n";
        exit(0);
    }
    std::string ip = argv[1];
    std::string port = argv[2];
    std::string pseudo = argv[3];
    User usr(ip, port, pseudo);
    bool running =true;
    while (running)
        {
            running = usr.actionsOnFds();
        }
    close(usr.sockfd);
    return 0;
}

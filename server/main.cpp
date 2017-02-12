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
#include "server.hpp"
int main(int argc, char *argv[])
{
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     Server serv(argv[1]);
     bool running = true;
     while(running) {
         // std::cout << "in boucle main server\n";
          running = serv.actionsOnFds();
     }
     close(serv.sockfd);
     return 0;
}

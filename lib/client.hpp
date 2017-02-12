#ifndef __CLIENT_H__
#define __CLIENT_H__
#include <string>
#include <netinet/in.h>

class Client
{

public:
    Client(int sockets, char* str);
    Client(int sockets);
		~Client();
    Client(const Client &) = delete;
    Client(const Client &&);
    int getSocket() const;
    std::string getPseudo() const;
    void setPseudo(char *pseudo);

protected:

    int _socket;
    std::string  _pseudo;
};

#endif // __CLIENT_H__

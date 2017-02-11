#ifndef __CLIENT_H__
#define __CLIENT_H__
#include <string>
#include <netinet/in.h>

class Client
{
protected:

    //std::string  _pseudo;
    int _socket;

public:
    Client(int sockets);
		~Client();
    int getSocket() const;
};

#endif // __CLIENT_H__

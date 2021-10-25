#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>

using namespace std;

class client
{
    static client* mpInstance;
    int server;

    public:
    // This is how clients can access the single instance
    static client* getInstance();
    void connectToServer(char *server_ip,char* server_port);
    void client_init(int argc, char **argv);

};

#endif

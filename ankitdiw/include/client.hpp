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
#include "../include/logger.h"
#include <vector>
#include <sstream>
#include "../include/utility.hpp"
#include "../include/commands.hpp"
#include <netinet/in.h>
//#include <regex>
//#include <conio.h>

using namespace std;

class client
{
    static client* mpInstance;
    int serverSocket;
    bool isServerConnected;
    std::string listeningPort;
    fd_set masterList,watchList;
    int headSocket;


    public:
    // This is how clients can access the single instance
    static client* getInstance();
    int connectToServer(const char *server_ip,const char* server_port);
    void client_init(int argc, char **argv);
    void handleStdinCmd();
    list<clientInfo> clientList;
    void handleServerMsg(char *buffer);
    int isValidClient(std::string clientIP);

};

#endif

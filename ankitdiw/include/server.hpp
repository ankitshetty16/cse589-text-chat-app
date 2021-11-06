#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <../include/commands.hpp>
#include <../include/utility.hpp>
#include <../include/logger.h>
#include <typeinfo>
#include <vector>

using namespace std;

class server { 
    static server* mpInstance;
    
    public:
    list<clientInfo> clientList;
    // This is how server can access the single instance
    static server* getInstance();
    void server_init(int argc, char **argv);
};

#endif

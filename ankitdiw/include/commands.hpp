#ifndef COMMANDS_HPP_
#define COMMANDS_HPP_

#include <stdio.h>
#include <string>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <../include/commands.hpp>
#include <../include/logger.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <netdb.h>
#include <bits/stdc++.h>

using namespace std;

class commands {
    public:
    void getAuthor(string command);
    void getIp(string command);
    void getPort(string port, string command);
    void addList(sockaddr_in client_addr);
    void removeList(sockaddr_in client_addr);
    void getList(string command);
    string returnList();
};

#endif

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

struct clientInfo
{
    string ip;
    string domain;
    int port;
    int socket_index;
};
string digitFormatter(int digit);
class commands {
    public:
    void getAuthor(string command);
    void getIp(string command);
    void getPort(string port, string command);
    void addList(list<clientInfo> &clientList, sockaddr_in client_addr,int socket_index, char* port);
    void removeList(list<clientInfo> &clientList, int socket_index);
    void getList(list<clientInfo> clientList, string command);
    string returnList(list<clientInfo> clientList);
    void transmitMsg(list<clientInfo> clientList,int sock_index, char* message, string type);
};

#endif

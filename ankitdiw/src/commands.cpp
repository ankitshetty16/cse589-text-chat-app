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

using namespace std;

#define S_ADDRESS "8.8.8.8"
#define S_PORT 53

/**
Function for generic commands
* 1. AUTHOR
* 2. IP
* 3. PORT
* 4. LIST
*/
int isUppercase(char* command){
    // std::string cmd = std::string(command);
    int i = 0;
    int uppercase = 1;
    while(command[i]){
        char l = command[i];
        if(!std::isupper(l)){
            uppercase = 0;
            break;
        };
    };
    return uppercase;
};
/**
* To get Author details (common func for server & client)
*/
void commands::getAuthor(string command){
    // if(isUppercase(command) == 0) {
    //     cse4589_print_and_log("[%s:ERROR]\n", "AUTHOR");
    // };

    cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
    cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n", "ankitdiw");
    cse4589_print_and_log("[%s:END]\n", command.c_str());
};

/**
* To get IP details (common func for server & client)
*/
void commands::getIp(string command){
    char myIP[16];
    unsigned int myPort;
    struct sockaddr_in serverAddress, myAddress;
    int sock;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Failed to open socket.");
        // exit(-1);
    }

    memset(&serverAddress, '\0', sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(S_ADDRESS);
    serverAddress.sin_port = htons(S_PORT);

    // Connect to server
    if (connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        perror("Connect server error");
        close(sock);
        // exit(-1);
    }

    memset(&myAddress, '\0' ,sizeof(myAddress));
    socklen_t len = sizeof(myAddress);
    getsockname(sock, (struct sockaddr *) &myAddress, &len);
    inet_ntop(AF_INET, &myAddress.sin_addr, myIP, sizeof(myIP));

    cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
    cse4589_print_and_log("IP:%s\n", myIP);
    cse4589_print_and_log("[%s:END]\n", command.c_str());
}

/**
* To get Port details (common func for server & client)
*/
void commands::getPort(string port,string command){
    cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
    cse4589_print_and_log("PORT:%s\n", port.c_str());
    cse4589_print_and_log("[%s:END]\n", command.c_str());
}
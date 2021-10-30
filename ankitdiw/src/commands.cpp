#include <stdio.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <../include/commands.hpp>
#include <../include/logger.h>

using namespace std;

    /**
    Function for generic commands
    1. AUTHOR
    2. IP
    3. PORT
    4. LIST
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

void commands::getAuthor(string command){
    // if(isUppercase(command) == 0) {
    //     cse4589_print_and_log("[%s:ERROR]\n", "AUTHOR");
    // };

    cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
    cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n", "ankitdiw");
    cse4589_print_and_log("[%s:END]\n", command.c_str());
};

void commands::getPort(string port,string command){
    cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
    cse4589_print_and_log("PORT:%d\n", port.c_str());
    cse4589_print_and_log("[%s:END]\n", command.c_str());
}
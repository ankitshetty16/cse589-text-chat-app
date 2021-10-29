#include <stdio.h>
#include <iostream>
#include <../include/commands.hpp>

using namespace std;

string commands::init(string type, char* command){
    printf("COMMANDS>>>>>>>>>>>>>  ");
    cout << command;
    // printf(command);
    return type;
};
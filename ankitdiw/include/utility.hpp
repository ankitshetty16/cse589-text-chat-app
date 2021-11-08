#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

using namespace std;
enum msgType
{
    AUTHOR,
    IP,
    PORT,
    LIST,
    STATISTICS,
    BLOCKED,
    LOGIN,
    REFRESH,
    SEND,
    BROADCAST,
    BLOCK,
    UNBLOCK,
    LOGOUT,
    EXIT,
    ADDCLIENT,  
    NOTFOUND
};

msgType getMsgType(std::string msg);



#endif
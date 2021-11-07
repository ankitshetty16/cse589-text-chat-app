#include "../include/utility.hpp"

msgType getMsgType(std::string msg)
{
    msgType m;
    if(msg == "AUTHOR")
    {
        m = AUTHOR;
        return m;
    }
    else if(msg == "IP")
    {
        m = IP;
        return m;
    }
    else if(msg == "PORT")
    {
        m = PORT;
        return m;
    }
    else if(msg == "LIST")
    {
        m = LIST;
        return m;
    }
    else if(msg == "STATISTICS")
    {
        m = STATISTICS;
        return m;
    }
    else if(msg == "BLOCKED")
    {
        m = BLOCKED;
        return m;
    }
    else if(msg == "LOGIN")
    {
        m = LOGIN;
        return m;
    }
    else if(msg == "REFRESH")
    {
        m = REFRESH;
        return m;
    }
    else if(msg == "SEND")
    {
        m = SEND;
        return m;
    }
    else if(msg == "BROADCAST")
    {
        m = BROADCAST;
        return m;
    }
    else if(msg == "BLOCK")
    {
        m = BLOCK;
        return m;
    }
    else if(msg == "UNBLOCK")
    {
        m = UNBLOCK;
        return m;
    }
    else if(msg == "LOGOUT")
    {
        m = LOGOUT;
        return m;
    }
    else if(msg == "EXIT")
    {
        m = EXIT;
        return m;
    }
    else if(msg == "~P")
    {
        m = ADDCLIENT;
        return m;
    }
    else
    {
        m = NOTFOUND;
        return m;
    }
    
        
} 
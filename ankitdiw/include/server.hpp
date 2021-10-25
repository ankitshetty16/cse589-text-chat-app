#ifndef SERVER_HPP_
#define SERVER_HPP_

class server { 
    static server* mpInstance;
    
    public:
    // This is how server can access the single instance
    static server* getInstance();
    void server_init(int argc, char **argv);
};

#endif

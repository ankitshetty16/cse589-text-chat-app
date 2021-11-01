
#include <../include/commands.hpp>

using namespace std;

#define S_ADDRESS "8.8.8.8"
#define S_PORT 53

class client
{
    public:
    string ip;
    string domain;
    int port;

    public:
    client(string ip, string domain, int port) {
        this->ip = ip;
        this->domain = domain;
        this->port = port;
    }
};

list<client> clientList;

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

/**
* To add to List of available clients
*/
void commands::addList(sockaddr_in client_addr){
        char myIP[16], host[1024], service[20];    
        unsigned int myPort;
        
        // To get client related details and add to list
        inet_ntop(AF_INET, &client_addr.sin_addr, myIP, sizeof(myIP));
        myPort = ntohs(client_addr.sin_port);
        getnameinfo((struct sockaddr*)&client_addr, sizeof(client_addr), host, sizeof(host), service, sizeof(service), 0);
        client c((string)myIP,host,myPort);
        clientList.push_back(c);

    // //For printing the list of servers available
    // int index = 0;
    // for(list<client>::iterator i = clientList.begin(); i != clientList.end(); i++){
    //     cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", index++, i -> domain.c_str(), i -> ip.c_str(), i -> port);
    // }
    // cse4589_print_and_log("[%s:SUCCESS]\n");
    // cse4589_print_and_log("IP:%s\n", myIP);
    // cse4589_print_and_log("[%s:END]\n","adsad");
}

/**
* To remove client from list of available clients
*/
void commands::removeList(sockaddr_in client_addr){
    printf("REMOVE LIST functionality");
}

/**
* To print the list of clients available
*/
void commands::getList(string command){
    // For printing the list of servers available
    int index = 0;
    cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
    for(list<client>::iterator i = clientList.begin(); i != clientList.end(); i++) {
        cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", index++, i -> domain.c_str(), i -> ip.c_str(), i -> port);
    }
    cse4589_print_and_log("[%s:END]\n", command.c_str());
}

/**
* To return the list of clients available
*/
string commands::returnList(){
    // for(list<client>::iterator i = clientList.begin(); i != clientList.end(); i++) {
    //     cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", index++, i -> domain.c_str(), i -> ip.c_str(), i -> port);
    // }
    return "TEST RETURN";
}
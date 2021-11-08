
#include <../include/commands.hpp>
#include <sstream>
#include <iomanip>

using namespace std;

#define S_ADDRESS "8.8.8.8"
#define S_PORT 53

// SORTING BY PORT
bool sortByPort(const clientInfo & a, const clientInfo & b) { return a.port < b.port; }

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

string digitFormatter(int digit) {
    stringstream ss;
    ss << setw(3) << setfill('0') << digit;
    string result = ss.str();
    return result;
}

/**
* Decode IP and client MSG
*/
void decodeMsg(char* encodedMsg, string &ip, string &message, string type){
    cout << "INTO DECODEMSG MESSAGE >>>>>>>>>" << endl;
	std::string buf = encodedMsg;
    if(type == "private"){
        cout << "INTO PRIVATE DECODE >>>>>>>>>" << endl;
        int ip_len = std::atoi(buf.substr(2,3).c_str());
        int pos = 5;
        ip = buf.substr(pos,ip_len);
        pos += ip_len;
        int msg_len = std::atoi(buf.substr(pos,3).c_str());
        pos += 3;
        message = buf.substr(pos,msg_len);
    }else if(type == "broadcast"){
        cout << "INTO BROADCAST DECODE >>>>>>>>>" << endl;
        int pos = std::atoi(buf.substr(2,3).c_str());
        int msg_len = std::atoi(buf.substr(pos,3).c_str());
        pos += 3;
        message = buf.substr(pos,msg_len);     
    }
}

/**
* Encode message and sender IP
*/
string msgEncoder(std::string ip,std::string message)
{
	std::string msg;
	msg = digitFormatter(ip.length())+ip+digitFormatter(message.length())+message;
	return msg;
}

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
void commands::addList(list<clientInfo> &clientList,sockaddr_in client_addr, int socket_index, char* port){
        char myIP[16], host[1024], service[20];    
        unsigned int myPort;
        
        // To get client related details and add to list
        inet_ntop(AF_INET, &client_addr.sin_addr, myIP, sizeof(myIP));
        myPort = atoi(port);
        getnameinfo((struct sockaddr*)&client_addr, sizeof(client_addr), host, sizeof(host), service, sizeof(service), 0);

        clientInfo response;
        response.ip = string(myIP);
        response.domain = host;
        response.port = myPort;
        response.socket_index = socket_index;

        clientList.push_back(response);
        clientList.sort(sortByPort);
}

/**
* To remove client from list of available clients
*/
void commands::removeList(list<clientInfo> &clientList, int socket_index){
    list<clientInfo>::iterator i = clientList.begin(); 
    while (i != clientList.end())
    {
        if (i->socket_index == socket_index){
            clientList.erase(i++);
        }
        else{
            ++i;
        }
    }
    clientList.sort(sortByPort);
}

/**
* To print the list of clients available
*/
void commands::getList(list<clientInfo> clientList,string command){
    // For printing the list of servers available
    int index = 1;
    cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
    for(list<clientInfo>::iterator i = clientList.begin(); i != clientList.end(); i++) {
        cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", index++, i -> domain.c_str(), i -> ip.c_str(), i -> port);
    }
    cse4589_print_and_log("[%s:END]\n", command.c_str());
}

/**
* To return the list of clients available to client on login
*/
string commands::returnList(list<clientInfo> clientList){
    string response;
    stringstream clSize;
    clSize << setw(1) << setfill('0') << clientList.size();    
    string start = "~L"+clSize.str();
    response.append(start.c_str());
    for(list<clientInfo>::iterator i = clientList.begin(); i != clientList.end(); i++) {
        string domain = i -> domain.c_str();
        string ip = i -> ip.c_str();
        stringstream port; 
        port << i -> port;
        string subString = digitFormatter(domain.length())+domain.c_str()+digitFormatter(ip.length())+ip.c_str()+digitFormatter(port.str().length())+port.str();
        response.append(subString);
    }
    return response;
}

/**
* Decode message and return 
*/
void commands::transmitMsg(list<clientInfo> clientList,int sock_index, char* message, string type){
    cout << "INTO PREPARE MESSAGE >>>>>>>>>" << endl;
    list<clientInfo>::iterator i = clientList.begin();
    string msg;
    string receiverIP;
    string response = "~M";
    string senderIP;
    int receiverSocket;
    
    decodeMsg(message,receiverIP,msg,type);
    cout << "INTO WHILE MESSAGE >>>>>>>>>" << endl;
    while (i != clientList.end())
    {
        if(type == "private" && i->ip == receiverIP){
            cout << "INTO PRIVTE WHILE MESSAGE >>>>>>>>>" << endl;
            receiverSocket = i->socket_index;
            cout <<receiverSocket << endl;
        }
        if (i->socket_index == sock_index){
            cout << "SENDER IP>>>>>>" << endl;
            cout << i->ip << endl;
            senderIP = i->ip;
            i++;
        }
        else{
            ++i;
        }
    }
    response.append(msgEncoder(senderIP,msg));
    int len;
    len = strlen(response.c_str());
    
    if(type == "private"){
        send(receiverSocket, response.c_str(), len, 0);
    }else if (type == "broadcast"){
        receiverIP = "255.255.255.255";
        list<clientInfo>::iterator i = clientList.begin();
        while (i != clientList.end()){
        	if(i->socket_index != sock_index){
        		send(i->socket_index, response.c_str(), len, 0);											
        		i++;
        	}else{
        		++i;
        	}
        }
    }

    cse4589_print_and_log("[%s:SUCCESS]\n", "RELAYED");
    cse4589_print_and_log("msg from:%s, to:%s\n[msg]:%s\n", senderIP.c_str(), receiverIP.c_str(), msg.c_str());
    cse4589_print_and_log("[%s:END]\n", "RELAYED");
}

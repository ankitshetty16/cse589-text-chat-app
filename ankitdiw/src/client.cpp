/**
* @client
* @author  Adithya Venkatesh <venkate9@buffalo.edu>
* @version 1.0
*
* @section LICENSE
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details at
* http://www.gnu.org/copyleft/gpl.html
*
* @section DESCRIPTION
*
* This file contains the client.
*  ./grader_controller -c ./grader.cfg -s ../venkate9_pa1.tar -t _list
*/

#include <../include/client.hpp>

#define TRUE 1
#define MSG_SIZE 256
#define BUFFER_SIZE 256
#define STDIN 0
#define FALSE 0

using namespace std;

client* client::mpInstance = NULL;
client* client::getInstance() 
{
   if (mpInstance == NULL) {
      mpInstance = new client();
   }
   return(mpInstance);
}

//int connect_to_host(char *server_ip, char *server_port);


void decodeListString(list<clientInfo> &clientList,char *buffer)
{	
	clientList.clear();
	std::string buf = buffer;
	int numClients = std::atoi(buf.substr(2,1).c_str());
	cout<<"Number of clients: "<<numClients<<endl;
	int pos = 3;
	for(int i = 0; i < numClients; ++i)
	{
		int d_len = std::atoi(buf.substr(pos,3).c_str());
		cout<<"Domain len="<<d_len<<"\n";
		pos += 3;
		std::string domain = buf.substr(pos,d_len);
		pos += d_len;
		cout<<"Domain:"<<domain<<"\n";

		int ip_len = std::atoi(buf.substr(pos,3).c_str());
		cout<<"ip len="<<ip_len<<"\n";
		pos += 3;
		std::string ip = buf.substr(pos,ip_len);
		pos += ip_len;
		cout<<"IP:"<<ip<<"\n";

		int port_len = std::atoi(buf.substr(pos,3).c_str());
		cout<<"port len="<<port_len<<"\n";
		pos += 3;
		int port = std::atoi(buf.substr(pos,port_len).c_str());
		pos += port_len;
		cout<<"PORT:"<<port<<"\n";

		clientInfo response;
    	response.ip = string(ip);
    	response.domain = string(domain);
    	response.port = port;

    	clientList.push_back(response);


	}
	
}
/**
* main function
*
* @param  argc Number of arguments
* @param  argv The argument list
* @return 0 EXIT_SUCCESS
*/

void client :: client_init(int argc, char **argv)
{
	if(argc != 3) {
		printf("Usage:%s [ip] [port]\n", argv[0]);
		exit(-1);
	}
	client* pClientobj = client::getInstance();
	memset(&pClientobj->serverSocket,0,sizeof(int));
	memset(&pClientobj->isServerConnected,0,sizeof(bool));
	
	std::vector<std::string> comArg;
	comArg.assign(argv + 1, argv + argc);
	pClientobj->listeningPort = comArg[1];
	/* Zero select FD sets */
	FD_ZERO(&pClientobj->masterList);
	FD_ZERO(&pClientobj->watchList);
	
	/* Register STDIN */
	FD_SET(STDIN, &pClientobj->masterList);
	
	pClientobj->headSocket = 0;
	printf("\n[PA1-CLIENT@CSE489/589]$ ");
	fflush(stdout);
	cout<<pClientobj->listeningPort<<"\n";
	int selret,sock_index;

	while(1)
	{	
		memcpy(&pClientobj->watchList, &pClientobj->masterList, sizeof(pClientobj->masterList));
		selret = select(pClientobj->headSocket + 1, &pClientobj->watchList, NULL, NULL, NULL);
		if(selret < 0)
			perror("select failed.");
		cout<<"Selret = "<<selret<<"\n";
		/* Check if we have sockets/STDIN to process */
		if(selret > 0)
		{	
			/* Loop through socket descriptors to check which ones are ready */
			for(sock_index=0; sock_index<=pClientobj->headSocket; sock_index+=1)
			{	
				if(FD_ISSET(sock_index, &pClientobj->watchList))
				{
					/* Check if new command on STDIN */
					if (sock_index == STDIN)
					{
						pClientobj->handleStdinCmd();
					}
					else if(sock_index == pClientobj->serverSocket)
					{
						//Handle once server is configured
						cout << "CHECKING SERVER SOCKET" << endl;
						char *buffer = (char*) malloc(sizeof(char)*BUFFER_SIZE);
                        memset(buffer, '\0', BUFFER_SIZE);
                        
                        if(recv(sock_index, buffer, BUFFER_SIZE, 0) <= 0){
                            close(sock_index);
                            printf("Remote Host terminated connection!\n");
                            // Remove client from list of clients
                            //cmdObj.removeList(client_addr);

                            /* Remove from watched list */
                            // FD_CLR(sock_index, &master_list);
                        }
                        else {
                            //Process incoming data from existing clients here ...
                            
                            printf("SERVER sent me: %s\n", buffer);
                            printf("ECHOing it back to the remote host ... \n");
							pClientobj->handleServerMsg(buffer);

                            //if(send(fdaccept, buffer, strlen(buffer), 0) == strlen(buffer))
                                // string test = "Beej was here!";
                                // int len, bytes_sent;
                                // len = strlen(test);
                                // bytes_sent = send(fdaccept, msg, len, 0);
                                // printf("Done!\n");
                            //fflush(stdout);
                        }
                        
                        free(buffer);						
					}
					else
					{
						cout << "INVALID SOCKET!!!!!!!!!!!!!!!!!!!!"<<endl;
					}
				}
			}
		}
		printf("\n[PA1-CLIENT@CSE489/589]$ ");
		fflush(stdout);
	}
}

int client :: connectToServer(const char *server_ip, const char* server_port)
{
	int fdsocket;
	struct addrinfo hints, *res;

	/* Set up hints structure */	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	/* Fill up address structures */	
	if (getaddrinfo(server_ip, server_port, &hints, &res) != 0)
	{
		perror("getaddrinfo failed");
		return FALSE;
	}
	/* Socket */
	fdsocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(fdsocket < 0)
	{
		perror("Failed to create socket");
		return FALSE;
	}
	/* Connect */
	if(connect(fdsocket, res->ai_addr, res->ai_addrlen) < 0)
	{
		perror("Connect failed");
		return FALSE;
	}
	freeaddrinfo(res);

	client::getInstance()->serverSocket = fdsocket;
	return TRUE;

}

int isValidIp(std::string ip)
{	
	struct sockaddr_in sa;
	
	// store this IP address in sa:
	//cout<<"received ip:"<<ip<<endl;
	int res = inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr));
	if(res == TRUE)
		return TRUE;
	else
		return FALSE;
}

int isValidPort(std::string port)
{
	for(int i = 0; i < port.length(); ++i)
	{
		if(!isdigit(port[i]))
		{
			return FALSE;
		}
	}
	return TRUE;
}

int client :: isValidClient(std::string clientIP)
{
	client* pClientobj = client::getInstance();
	for(list<clientInfo>::iterator i = pClientobj->clientList.begin(); i != pClientobj->clientList.end(); i++) 
	{
		if(clientIP == i->ip)
			return TRUE;
    }
	return FALSE;
}

std::string encodeMsg(std::string ip,std::string message)
{
	std::string msg;
	msg = "~M"+digitFormatter(ip.length())+ip+digitFormatter(message.length())+message;
	return msg;
}

void client :: handleStdinCmd()
{	
	//cout<<"Inside HandleStdin\n";
	client* pClientobj = client::getInstance();
	std::vector<std::string> commandArgv;
	commands cmdObj;
	commandArgv.clear();
	std::string command;
	std::getline(std::cin,command);
	int arg_count = 0;
	std::istringstream iss(command);
	std::string token;
	std::string sendPortNum;
	std::string sendMsgBuf;
	while(std::getline(iss, token, ' '))
	{
		commandArgv.push_back(token);
		arg_count += 1;
	}
	msgType msg = getMsgType(commandArgv[0]);
	switch(msg)
	{
		case AUTHOR:   	
			cmdObj.getAuthor(commandArgv[0]);
			break;
		case PORT:
			cmdObj.getPort(pClientobj->listeningPort, commandArgv[0]);
			break;
		case IP:
			cmdObj.getIp(commandArgv[0]);
			break;
		case LOGIN:
			if(!isValidIp(commandArgv[1]))
			{
				cse4589_print_and_log("[%s:ERROR]\n", commandArgv[0].c_str());
    			cse4589_print_and_log("[%s:END]\n", commandArgv[0].c_str());
				break;
			}
			if(!isValidPort(commandArgv[2]))
			{	
				cout<<"it's a valid ip\n";
				cse4589_print_and_log("[%s:ERROR]\n", commandArgv[0].c_str());
    			cse4589_print_and_log("[%s:END]\n", commandArgv[0].c_str());
				break;
			}
			if(!pClientobj->connectToServer(commandArgv[1].c_str(),commandArgv[2].c_str()))
			{
				cse4589_print_and_log("[%s:ERROR]\n", commandArgv[0].c_str());
    			cse4589_print_and_log("[%s:END]\n", commandArgv[0].c_str());
				break;
			}
			if(pClientobj->isServerConnected)
			{
				cse4589_print_and_log("[%s:ERROR]\n", commandArgv[0].c_str());
    			cse4589_print_and_log("[%s:END]\n", commandArgv[0].c_str());
				break;
			}
			pClientobj->isServerConnected = TRUE;
			FD_SET(pClientobj->serverSocket, &pClientobj->masterList);
			pClientobj->headSocket = pClientobj->serverSocket + 1;
			sendPortNum = "~P" + pClientobj->listeningPort;
			//sendPortNum = "~P" + pClientobj->listeningPort;
			send(pClientobj->serverSocket, sendPortNum.c_str(), sendPortNum.length(), 0);	
			cse4589_print_and_log("[%s:SUCCESS]\n", commandArgv[0].c_str());
    		cse4589_print_and_log("[%s:END]\n", commandArgv[0].c_str());
			cout<<"Connected to server\n";
			break;

		case LIST:
			if(!pClientobj->isServerConnected)
			{
				cse4589_print_and_log("[%s:ERROR]\n", commandArgv[0].c_str());
    			cse4589_print_and_log("[%s:END]\n", commandArgv[0].c_str());
				break;
			}
			cmdObj.getList(pClientobj->clientList,commandArgv[0]);
			break;

		case SEND:
			if(!pClientobj->isServerConnected)
			{
				cse4589_print_and_log("[%s:ERROR]\n", commandArgv[0].c_str());
    			cse4589_print_and_log("[%s:END]\n", commandArgv[0].c_str());
				break;
			}
			if(!isValidIp(commandArgv[1]))
			{
				cse4589_print_and_log("[%s:ERROR]\n", commandArgv[0].c_str());
    			cse4589_print_and_log("[%s:END]\n", commandArgv[0].c_str());
				break;
			}
			if(!(pClientobj->isValidClient(commandArgv[1])))
			{
				cse4589_print_and_log("[%s:ERROR]\n", commandArgv[0].c_str());
    			cse4589_print_and_log("[%s:END]\n", commandArgv[0].c_str());
				break;
			}

			printf("\nSENDing it to the remote server ... \n");
			sendMsgBuf = encodeMsg(commandArgv[1],commandArgv[2]);
			cout<<"Encoded msg : --> "<<endl;
			if(send(pClientobj->serverSocket, sendMsgBuf.c_str(), sendMsgBuf.length(), 0) > 0)
			{
				printf("Done!\n");
				cse4589_print_and_log("[%s:SUCCESS]\n", commandArgv[0].c_str());
				cse4589_print_and_log("[%s:END]\n", commandArgv[0].c_str());
			}
			else
			{	
				cout<<"Msg not sent to server\n";
				cse4589_print_and_log("[%s:ERROR]\n", commandArgv[0].c_str());
    			cse4589_print_and_log("[%s:END]\n", commandArgv[0].c_str());
			}
			break;
		case LOGOUT:
			if(!pClientobj->isServerConnected)
			{
				cse4589_print_and_log("[%s:ERROR]\n", commandArgv[0].c_str());
    			cse4589_print_and_log("[%s:END]\n", commandArgv[0].c_str());
				break;
			}
			close(pClientobj->serverSocket);
			printf("Remote Host terminated connection!\n");
			// Remove client from list of clients

			/* Remove from watched list */
			pClientobj->clientList.clear();
			FD_CLR(pClientobj->serverSocket, &pClientobj->masterList);
			pClientobj->headSocket = 0;
			pClientobj->isServerConnected = FALSE;
			cse4589_print_and_log("[%s:SUCCESS]\n", commandArgv[0].c_str());
    		cse4589_print_and_log("[%s:END]\n", commandArgv[0].c_str());
			break;
		case EXIT:
			if(pClientobj->isServerConnected)
			{
				close(pClientobj->serverSocket);
				FD_CLR(pClientobj->serverSocket, &pClientobj->masterList);
			}
			cse4589_print_and_log("[%s:SUCCESS]\n",commandArgv[0].c_str());
			cse4589_print_and_log("[%s:END]\n", commandArgv[0].c_str());
			exit(0);
			break;

	}
}


void handleRcvdMsgFromCLient(char *buffer)
{	
	std::string buf = buffer;
	int ip_len = std::atoi(buf.substr(2,3).c_str());
	int pos = 5;
	std::string ip = buf.substr(pos,ip_len);
	pos += ip_len;
	int msg_len = std::atoi(buf.substr(pos,3).c_str());
	pos += 3;
	std::string msg = buf.substr(pos,msg_len);
	cse4589_print_and_log("[%s:SUCCESS]\n","RECEIVED");
	cse4589_print_and_log("msg from:%s\n[msg]:%s\n", ip.c_str(), msg.c_str());
	cse4589_print_and_log("[%s:END]\n", "RECEIVED");
}

void client :: handleServerMsg(char *buffer)
{	
	client* pClientobj = client::getInstance();
	if(buffer[0] == '~' && buffer[1] == 'L')
	{
		cout<<"It is a list msg\n";
		decodeListString(pClientobj->clientList,buffer);
	}
	else if(buffer[0] == '~' && buffer[1] == 'M')
	{
		cout<<"It is regular msg from client\n";
		handleRcvdMsgFromCLient(buffer);
	}
}
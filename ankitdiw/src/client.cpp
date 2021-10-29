/**
* @client
* @author  Swetank Kumar Saha <swetankk@buffalo.edu>, Shivang Aggarwal <shivanga@buffalo.edu>
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
*/

#include <../include/client.hpp>

#define TRUE 1
#define MSG_SIZE 256
#define BUFFER_SIZE 256

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
	commands cmdObj;
	cout << "Inside client init"<<endl;
	//connect_to_host(argv[1], argv[2]);
	/*
	pClientobj->connectToServer("127.0.0.1","4566");
	
	while(TRUE){
		printf("\n[PA1-Client@CSE489/589]$ ");
		fflush(stdout);
		
		char *msg = (char*) malloc(sizeof(char)*MSG_SIZE);
		memset(msg, '\0', MSG_SIZE);
		if(fgets(msg, MSG_SIZE-1, stdin) == NULL) //Mind the newline character that will be written to msg
			exit(-1);
		
		printf("I got: %s(size:%d chars)", msg, strlen(msg));
		
		printf("\nSENDing it to the remote server ... ");
		if(send(server, msg, strlen(msg), 0) == strlen(msg))
			printf("Done!\n");
		fflush(stdout);
		
		/* Initialize buffer to receieve response 
		char *buffer = (char*) malloc(sizeof(char)*BUFFER_SIZE);
		memset(buffer, '\0', BUFFER_SIZE);
		
		if(recv(server, buffer, BUFFER_SIZE, 0) >= 0){
			printf("Server responded: %s", buffer);
			fflush(stdout);
		}
		*/
	memset(&pClientobj->server,0,sizeof(int));
	memset(&pClientobj->isServerConnected,0,sizeof(bool));
	//memset(&pClientobj->listeningPort,0,sizeof(int));
	cout << " before listen port"<<endl;
	// std:: string port = argv[2];
	// cout <<"portt------>";
	// cout << port;
	// cout << strlen(argv[2]);
	// cout << argv[2];
	// cout << *argv[2];
	//pClientobj->listeningPort = argv[2];
	std::vector<std::string> comArg;
	comArg.assign(argv + 1, argv + argc);
	cout<<"comArg----->"<<endl;
	cout<<comArg[0];
	cout<<comArg[1];
	pClientobj->listeningPort = comArg[1];
	//cout <<"after listen port \n";
	std::vector<std::string> commandArgv;
	while(1)
	{	
		//cout<<"inside while\n";
		commandArgv.clear();
		std::string command;
		std::getline(std::cin,command);
		int arg_count = 0;
		std::istringstream iss(command);
		std::string token;
		//char* arg = strtok(&command.c_str()," ");
		while(std::getline(iss, token, ' '))
		{
			commandArgv.push_back(token);
			arg_count += 1;
		}
		msgType msg = getMsgType(commandArgv[0]);
		cout<<"MSG TYPE ------------>"<<msg<<endl;
		switch(msg)
		{
			case AUTHOR:   	cse4589_print_and_log("[%s:SUCCESS]\n", commandArgv[0].c_str());
							//cmdObj.getAuthor();
						    break;
			case PORT:      cse4589_print_and_log("[%s:SUCCESS]\n", commandArgv[0].c_str());
							cse4589_print_and_log("[%s:PORT]\n", pClientobj->listeningPort.c_str());
							//cmdObj.getPort(pClientobj->listeningPort);
							break;
			case IP:      
							// struct addrinfo hints,*res
							// memset(&hints, 0, sizeof(hints));
							// hints.ai_family = AF_INET;
							// hints.ai_socktype = SOCK_STREAM;
							// char* server_ip = "8.8.8.8"
							// char* server_port = "53"
							// if (getaddrinfo(server_ip, server_port, &hints, &res) != 0)
							// 	perror("getaddrinfo failed");
							// /* Socket */
							// fdsocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
							// if(fdsocket < 0)
							// 	perror("Failed to create socket");
	
							// /* Connect */
							// if(connect(fdsocket, res->ai_addr, res->ai_addrlen) < 0)
							// 	perror("Connect failed");
							break;
			case EXIT: 	cse4589_print_and_log("[%s:SUCCESS]\n",commandArgv[0].c_str());
						 	return;

		}

	}
}

void client :: connectToServer(char *server_ip, char* server_port)
{
	int fdsocket;
	struct addrinfo hints, *res;

	cout<<"inside connect to server"<<endl;
	/* Set up hints structure */	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	/* Fill up address structures */	
	if (getaddrinfo(server_ip, server_port, &hints, &res) != 0)
		perror("getaddrinfo failed");

	/* Socket */
	fdsocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(fdsocket < 0)
		perror("Failed to create socket");
	
	/* Connect */
	if(connect(fdsocket, res->ai_addr, res->ai_addrlen) < 0)
		perror("Connect failed");
	
	freeaddrinfo(res);

	client::getInstance()->server = fdsocket;
}

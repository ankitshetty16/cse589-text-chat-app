/**
* @server
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
* This file contains the server init and main while loop for tha application.
* Uses the select() API to multiplex between network I/O and STDIN.
*/
#include <../include/server.hpp>

#define BACKLOG 5
#define STDIN 0
#define TRUE 1
#define CMD_SIZE 100
#define BUFFER_SIZE 256

server* server::mpInstance = NULL;
commands cmdObj;
server* server::getInstance() 
{
   if (mpInstance == NULL) {
      mpInstance = new server();
   }
   return(mpInstance);
}

/**
* main function
*
* @param  argc Number of arguments
* @param  argv The argument list
* @return 0 EXIT_SUCCESS
*/
void server :: server_init(int argc, char **argv)
{	
	if(argc != 3) {
		printf("Usage:%s [port]\n", argv[1]);
		exit(-1);
	}
	
	server* pServerobj = server::getInstance();
	int server_socket, head_socket, selret, sock_index, fdaccept=0;
    socklen_t caddr_len;
	struct sockaddr_in client_addr;
	struct addrinfo hints, *res;
	fd_set master_list, watch_list;

	/* Set up hints structure */
	memset(&hints, 0, sizeof(hints));
    	hints.ai_family = AF_INET;
    	hints.ai_socktype = SOCK_STREAM;
    	hints.ai_flags = AI_PASSIVE;

	/* Fill up address structures */
	if (getaddrinfo(NULL, argv[2], &hints, &res) != 0)
		perror("getaddrinfo failed");
	
	/* Socket */
	server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(server_socket < 0)
		perror("Cannot create socket");
	
	/* Bind */
	if(bind(server_socket, res->ai_addr, res->ai_addrlen) < 0 )
		perror("Bind failed");

	freeaddrinfo(res);
	
	/* Listen */
	printf("\n[PA1-SERVER@CSE489/589]$ ");
	fflush(stdout);	
	if(listen(server_socket, BACKLOG) < 0)
		perror("Unable to listen on port");
	/* ---------------------------------------------------------------------------- */
	/* Zero select FD sets */
	FD_ZERO(&master_list);
	FD_ZERO(&watch_list);
	
	/* Register the listening socket */
	FD_SET(server_socket, &master_list);
	/* Register STDIN */
	FD_SET(STDIN, &master_list);
	
	head_socket = server_socket;
	while(TRUE){
		memcpy(&watch_list, &master_list, sizeof(master_list));
		/* select() system call. This will BLOCK */
		selret = select(head_socket + 1, &watch_list, NULL, NULL, NULL);
		if(selret < 0)
			perror("select failed.");

		/* Check if we have sockets/STDIN to process */
		if(selret > 0){
			/* Loop through socket descriptors to check which ones are ready */
			for(sock_index=0; sock_index<=head_socket; sock_index+=1){
				
				if(FD_ISSET(sock_index, &watch_list)){
					/* Check if new command on STDIN */
					if (sock_index == STDIN){
						char *cmd = (char*) malloc(sizeof(char)*CMD_SIZE);
						
						memset(cmd, '\0', CMD_SIZE);
						if(fgets(cmd, CMD_SIZE-1, stdin) == NULL) //Mind the newline character that will be written to cmd
							exit(-1);
						
						printf("\nI got: %s\n", cmd);

						//Process PA1 commands here ...
						char *arg = strtok(cmd," ");
						std::string token;
						std::vector<std::string> cmdArgv;
						while(arg != NULL){
							token = arg;
							int len = token.length();
							if(token[len-1] == '\n')
							{
								token.erase(len-1);
							}
							cmdArgv.push_back(token);
							arg = strtok(NULL," ");
						}
						msgType msg;
						memset(&msg, 0, sizeof(msgType));
						msg = getMsgType(cmdArgv[0]);
						//Processing PA1 SERVER COMMANDS Here
						switch(msg) {
							case AUTHOR:
								cmdObj.getAuthor(cmdArgv[0]);
								break;
							case PORT:
								cmdObj.getPort(argv[2], cmdArgv[0]);
								break;
							case IP:
								cmdObj.getIp(cmdArgv[0]);
								break;
							case LIST:
								cmdObj.getList(pServerobj->clientList, cmdArgv[0]);
								break;			
							case NOTFOUND:
								cse4589_print_and_log("[%s:ERROR]\n", cmdArgv[0].c_str());
								cse4589_print_and_log("[%s:END]\n", cmdArgv[0].c_str());
								break;
						}
						free(cmd);
					}
					/* Check if new client is requesting connection */
					else if(sock_index == server_socket){
						caddr_len = sizeof(client_addr);
						fdaccept = accept(server_socket, (struct sockaddr *)&client_addr, &caddr_len);
						if(fdaccept < 0)
							perror("Accept failed.");
						
						printf("\nRemote Host connected!");                      
						            
						/* Add to watched socket list */
						FD_SET(fdaccept, &master_list);
						if(fdaccept > head_socket) head_socket = fdaccept;						
					}
					/* Read from existing clients */
					else{
						/* Initialize buffer to receieve response */
						char *buffer = (char*) malloc(sizeof(char)*BUFFER_SIZE);
						memset(buffer, '\0', BUFFER_SIZE);
						
						if(recv(sock_index, buffer, BUFFER_SIZE, 0) <= 0){
							// Remove client from list of clients
							cmdObj.removeList(pServerobj->clientList, sock_index);
							close(sock_index);
							printf("Remote Host terminated connection!\n");

							/* Remove from watched list */
							FD_CLR(sock_index, &master_list);
						}
						else {
							//Process incoming data from existing clients here ...
							printf("\nClient sent me: %s\n", buffer);
							printf("ECHOing it back to the remote host ... \n");
							cout << buffer[0]+buffer[1] << endl;

							msgType msg;
							char pCmd[2] = {buffer[0],buffer[1]};
							msg = getMsgType(pCmd);
							switch(msg){
								case ADDCLIENT:
									// Add new client to list of clients
									char port[4];
									strncpy(port,buffer+2,4);
									cmdObj.addList(pServerobj->clientList,client_addr,fdaccept, port);
									// return list of avilable clients on login
									string response = cmdObj.returnList(pServerobj->clientList);
									int len;
									len = strlen(response.c_str());
									send(fdaccept, response.c_str(), len, 0);
									break;
							}

							if(send(fdaccept, buffer, strlen(buffer), 0) == strlen(buffer))
								printf("Done!\n");
							fflush(stdout);
						}
						
						free(buffer);
					}
				}
			}
		}
	printf("\n[PA1-SERVER@CSE489/589]$ ");
	fflush(stdout);
	}
}

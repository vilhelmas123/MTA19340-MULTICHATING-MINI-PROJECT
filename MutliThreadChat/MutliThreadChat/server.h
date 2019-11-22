#ifndef _server_h_
#define _server_h_

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <ws2tcpip.h>

#include "mythread.h"
#include "client.h"

constexpr auto PORT = 54000;

using namespace std;

class Server {
private:
	static vector<Client> clients;
	SOCKET serverSocket;
	struct sockaddr_in serverAddr, clientAddr;
	char buff[256];

public:
	Server();
	void mainLoop();
	static void * clientCtrl(void *args);

private:
	static void getClientList();
	static void sendToAll(char *message);
	static int getClientIndex(Client *c); 
};

#endif

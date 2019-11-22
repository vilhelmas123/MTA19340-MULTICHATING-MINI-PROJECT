#include "server.h"

using namespace std;

//Allocation
vector<Client> Server::clients;

Server::Server() {
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	if (WSAStartup(ver, &wsData) != 0) {
		cerr << "Failed to initialize winsock." << endl;
		return;
	}

	//Initialize mutex
	myThread::initMutex();

	//Initialize serverSocket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET) {
		cerr << "Failed to create a socket." << endl;
		return;
	}

	ZeroMemory(&serverAddr, sizeof(sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);

	//Avoid bind errors
	char y = 1;
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int));

	if (bind(serverSocket, (sockaddr*)& serverAddr, sizeof(sockaddr_in)) < 0) {
		cerr << "Failed to bind." << endl;
		return;
	}

	//Listening
	listen(serverSocket, 5);
}

void Server::mainLoop() {
	Client* c;
	myThread* t;
	socklen_t addrLen = sizeof(sockaddr_in);

	while (1) {
		c = new Client();
		t = new myThread();

		c->socket = accept(serverSocket, (struct sockaddr*) & clientAddr, &addrLen);

		if (c->socket < 0)
			cerr << "Failed to accept.";
		else t->create((void*)
			Server::clientCtrl, c);
	}
}

void* Server::clientCtrl(void* args) {
	//Pointer to accepted client
	Client* c = (Client*)args;
	char buffer[256], msg[256];
	int index;
	int n;

	//Put client to cliens vector
	myThread::lockMutex((const char*)c->name);

	//Give client an id
	c->SetId(Server::clients.size());
	sprintf_s(buffer, "Client n.%d", c->id);
	c->SetName(buffer);
	cout << "Adding client with id: " << c->id << endl;
	Server::clients.push_back(*c);

	myThread::unlockMutex((const char*)c->name);

	while (1) {
		memset(buffer, 0, 256);
		n = recv(c->socket, buffer, sizeof(buffer), 0);

		//Client disconnected - erase client, end thread
		if (n == 0) {
			cout << "Client " << c->name << " diconnected." << endl;
			closesocket(c->socket);

			myThread::lockMutex((const char*)c->name);

			index = Server::getClientIndex(c);
			cout << "Deleting user with index: " << index << ",  id: " << Server::clients[index].id << endl;
			Server::clients.erase(Server::clients.begin() + index);

			myThread::unlockMutex((const char*)c->name);

			break;
		}

		else if (n < 0)
			cerr << "Failed to receive client's message: " << c->name << endl;

		//Message received - send to all clients
		else  {
			snprintf(msg, sizeof msg, "<%s> %s \r", c->name, buffer);
			cout << "Sending to all: " << msg << endl;
			Server::sendToAll(msg);
		}
	}

	//End thread
	return NULL;
}

void Server::sendToAll(char* msg) {
	//Get the lock
	myThread::lockMutex("sendToAll()");

	for (size_t i = 0; i < clients.size(); i++)
		send(Server::clients[i].socket, msg, strlen(msg), 0);

	//Release the lock  
	myThread::unlockMutex("sendToAll()");
}

void Server::getClientList() {
	for (size_t i = 0; i < clients.size(); i++)
		cout << clients.at(i).name << endl;
}

int Server::getClientIndex(Client* c) {
	for (size_t i = 0; i < clients.size(); i++)
		if ((Server::clients[i].id) == c->id)
			return (int)i;

	cerr << "Client id not found." << endl;
	return -1;
}

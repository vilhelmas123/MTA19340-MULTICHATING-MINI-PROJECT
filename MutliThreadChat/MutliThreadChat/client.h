#ifndef _client_h_
#define _client_h_

#include <iostream>

#include <cstdio>
#include <cstdlib>
#include <ws2tcpip.h>

constexpr auto MAX_NAME_LENGHT = 20;

using namespace std;

class Client {
public:
	//Variables (have to be public)
	char* name;
	int id;
	SOCKET socket;

	//Methods
	Client();
	void SetName(const char* name);
	void SetId(int id);
};

#endif

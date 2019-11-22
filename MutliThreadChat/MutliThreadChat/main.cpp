#include <iostream>
#include "mythread.h"
#include "server.h"

using namespace std;

int main() {
	cout << "Starting..." << endl;
	Server* s;
	s = new Server();
	s->mainLoop();
	return 0;
}

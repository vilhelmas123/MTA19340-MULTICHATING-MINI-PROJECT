#ifndef _mythread_h_
#define _mythread_h_

#include <iostream>
#include <string>

#include "pthread.h"
#include <io.h>

#include <cstdlib>

using namespace std;

class myThread {
public:
	pthread_t thr;

private:
	static pthread_mutex_t mutex;

public:
	myThread();
	int create(void *Callback, void *args);
	int join();

	static int initMutex();
	static int lockMutex(const char *identifier);
	static int unlockMutex(const char *identifier);
};

#endif

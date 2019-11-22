#include "myThread.h"

using namespace std;

//Allocation
pthread_mutex_t myThread::mutex;

myThread::myThread() { }

int myThread::create(void* Callback, void* args) {
	if (pthread_create(&this->thr, NULL, (void* (*)(void*))Callback, args)) {
		cerr << "Failed to create threads." << endl;
		return pthread_create(&this->thr, NULL, (void* (*)(void*))Callback, args);
	}
	else {
		cout << "Thread created." << endl;
		return 0;
	}
}

int myThread::join() {
	pthread_join(this->thr, NULL);
	return 0;
}

int myThread::initMutex() {
	if (pthread_mutex_init(&myThread::mutex, NULL) < 0) {
		cerr << "Failed to initialize mutex." << endl;
		return -1;
	}
	else {
		cout << "Mutex initialized." << endl;
		return 0;
	}
}

//Block until mutex is usable
int myThread::lockMutex(const char* id) {
	cout << id << " is trying to get the lock." << endl;
	if (pthread_mutex_lock(&myThread::mutex) == 0) {
		cout << id << " got the lock." << endl;
		return 0;
	}
	else {
		cerr << id << " failed to get the lock." << endl;
		return -1;
	}
}

int myThread::unlockMutex(const char* id) {
	cout << id << " is trying to release the lock." << endl;
	if (pthread_mutex_unlock(&myThread::mutex) == 0) {
		cout << id << " released the lock." << endl;
		return 0;
	}
	else {
		cerr << id << " failed to release the lock." << endl;
		return -1;
	}
}
#pragma once

#include "Noncopyable.h"
#include <pthread.h>
#include <functional>

namespace mm
{

class Thread
: Noncopyable
{
public:
	using ThreadCallback = std::function<void()>;
	Thread(ThreadCallback && cb);

	void start();
	void join();

	~Thread();

private:
	static void * threadFunc(void *);

private:
	pthread_t _pthid;
	ThreadCallback _cb;
	bool _isRunning; 
};

}//end of namespace mm


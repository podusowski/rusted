#pragma once

#include <pthread.h>

#include "IRunnable.hpp"
#include "Mutex.hpp"

namespace Cake 
{
namespace Threading
{

class Thread
{
public:
    struct Context
    {
        Thread * thread;
        IRunnable * runnable;
    };

	Thread(IRunnable &);
	~Thread();

	void start();
	void join();
	bool isRunning();

	static void wait(int secs, int mili = 0);
	static unsigned self();
private:
	static void * m_run(void *);

    bool m_isRunning;
	IRunnable & m_runnable;
	pthread_t m_thread;
	Mutex m_isRunningLock;
};

}
}

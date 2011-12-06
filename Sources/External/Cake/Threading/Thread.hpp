#pragma once

#include <pthread.h>

#include "IRunnable.hpp"

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
	bool isRunning();

	static void wait(float p_secs);
	static unsigned self();
private:
	static void * m_run(void *);

    bool m_isRunning;
	IRunnable & m_runnable;
	pthread_t m_thread;
};

}
}

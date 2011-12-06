#ifndef COMMON_THREAD_HPP
#define COMMON_THREAD_HPP

#include <pthread.h>

#include "IRunnable.hpp"

namespace Common
{

/**
 * Common thread library.
 */
class Thread
{
public:
	Thread(IRunnable &);
	~Thread();

	/**
	 * Execute the runnable object in separate thread.
	 */
	void start();

	static void wait(float p_secs);
private:
	static void * m_run(void *);

	IRunnable & m_runnable;
	pthread_t m_thread;
};

}

#endif

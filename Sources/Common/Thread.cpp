#include <cmath>

#include "Thread.hpp"

Common::Thread::Thread(Common::IRunnable & p_runnable) :
	m_runnable(p_runnable) { }

Common::Thread::~Thread()
{
}

void Common::Thread::start()
{
	pthread_create(&m_thread, NULL, Common::Thread::m_run, (void *)&m_runnable);
}

void Common::Thread::wait(float p_secs)
{
	timespec l_ts;
	l_ts.tv_sec = floor(p_secs);
	l_ts.tv_nsec = ( p_secs - l_ts.tv_sec ) * 1000;
	nanosleep(&l_ts, 0);
}

void * Common::Thread::m_run(void * p_runnable)
{
	static_cast<Common::IRunnable *>(p_runnable)->run();
	return 0;
}

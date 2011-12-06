#include <cmath>
#include <cerrno>
#include <stdexcept>
#include <sstream>

#include "Thread.hpp"

using namespace Cake::Threading;

Thread::Thread(IRunnable & p_runnable) :
	m_isRunning(false),
	m_runnable(p_runnable)
{
}

Thread::~Thread()
{
}

void Thread::start()
{
	m_isRunning = true;
	int ret = pthread_create(&m_thread, NULL, Thread::m_run, (void *)this);

    if (ret == EAGAIN)
    {
        throw std::runtime_error("no system resources to create thread");
    }
    else if (ret == EINVAL)
    {
        throw std::runtime_error("invalid pthread attr");
    }
    else if (ret == EPERM)
    {
        throw std::runtime_error("no permission to create a thread");
    }
    else if (ret != 0)
	{
	    std::stringstream ss;
	    ss << "error during pthread_create: " << ret;
        throw std::runtime_error(ss.str());
	}
}

void Thread::wait(float p_secs)
{
	timespec l_ts;
	l_ts.tv_sec = floor(p_secs);
	l_ts.tv_nsec = ( p_secs - l_ts.tv_sec ) * 1000;
	nanosleep(&l_ts, 0);
}

unsigned Thread::self()
{
    return pthread_self();
}

void * Thread::m_run(void * threadCtx)
{
    Thread & thread = *static_cast<Thread *>(threadCtx);

	thread.m_runnable.run();
	thread.m_isRunning = false;
	pthread_detach(pthread_self());

	return 0;
}

bool Thread::isRunning()
{
    return m_isRunning;
}

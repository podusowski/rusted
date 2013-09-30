#include <cmath>
#include <cerrno>
#include <stdexcept>
#include <sstream>

#include "Thread.hpp"
#include "ScopedLock.hpp"

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

#ifdef _WIN32
    DWORD id;
    m_thread = CreateThread(
        0, // Security attributes
        0, // Stack size
        pFun,
        (void *)this,
        CREATE_SUSPENDED,
        &id);
#else
    int ret = pthread_create(&m_thread, NULL, Thread::s_run, (void *)this);

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
#endif
}

void Thread::join()
{
#ifdef _WIN32
#else
    pthread_join(m_thread, 0);
#endif
}

void Thread::wait(int secs, int mili)
{
    timespec l_ts;
    l_ts.tv_sec = secs;
    l_ts.tv_nsec = mili * 1000 * 1000;
    nanosleep(&l_ts, 0);
}

unsigned Thread::self()
{
    return pthread_self();
}

void * Thread::s_run(void * threadCtx)
{
    Thread & thread = *static_cast<Thread *>(threadCtx);

    try
    {
        thread.m_runnable.run();
    }
    catch (...)
    {
    }

    // basically bool should be atomic but we'll be valgrind clean
    thread.m_isRunningLock.aquire();
    thread.m_isRunning = false;
    thread.m_isRunningLock.release();

    pthread_detach(pthread_self());

    return 0;
}

bool Thread::isRunning()
{
    ScopedLock lock(m_isRunningLock);
    return m_isRunning;
}


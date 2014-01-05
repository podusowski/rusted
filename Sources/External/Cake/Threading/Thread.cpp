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
        LPTHREAD_START_ROUTINE(&Thread::s_run),
        (void *)this,
        0,
        &id);

    if (m_thread == NULL)
    {
        throw std::runtime_error("CreateThread returned NULL");
    }
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
    WaitForSingleObject(m_thread, INFINITE);
#else
    pthread_join(m_thread, 0);
#endif
}

void Thread::wait(int secs, int mili)
{
#ifdef _WIN32
    Sleep(mili + (secs * 100));
#else
    timespec l_ts;
    l_ts.tv_sec = secs;
    l_ts.tv_nsec = mili * 1000 * 1000;
    nanosleep(&l_ts, 0);
#endif
}

unsigned Thread::self()
{
#ifdef _WIN32
    return GetCurrentThreadId();
#else
    return pthread_self();
#endif
}

#ifdef _WIN32
    DWORD Thread::s_run(void * threadCtx)
#else
    void * Thread::s_run(void * threadCtx)
#endif
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

#ifndef _WIN32
    pthread_detach(pthread_self());
#endif

    return 0;
}

bool Thread::isRunning()
{
    ScopedLock lock(m_isRunningLock);
    return m_isRunning;
}


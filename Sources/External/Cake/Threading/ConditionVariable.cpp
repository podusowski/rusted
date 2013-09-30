#include <sys/time.h>
#include <stdexcept>

#include "ConditionVariable.hpp"

using namespace Cake::Threading;

ConditionVariable::ConditionVariable(Mutex & mutex) : m_mutex(mutex)
{
#ifdef _WIN32
    #error not implemented
#else
    pthread_cond_init(&m_condition, 0);
#endif
}

ConditionVariable::~ConditionVariable()
{
#ifdef _WIN32
    #error not implemented
#else
    pthread_cond_destroy(&m_condition);
#endif
}

void ConditionVariable::wait()
{
#ifdef _WIN32
    #error not implemented
#else
    int ret = pthread_cond_wait(&m_condition, m_mutex.getNativeHandle());

    if (ret == EPERM)
    {
        throw std::runtime_error("condition variable mutex in not locked");
    }
#endif
}

ETimedWaitResult ConditionVariable::timedWait(unsigned seconds, unsigned miliseconds)
{
#ifdef _WIN32
    #error not implemented
#else
    timeval c;
    gettimeofday(&c, NULL);

    timespec t;
    t.tv_sec = c.tv_sec + seconds;
    t.tv_nsec = /* to nsec */(c.tv_usec * 1000) + /* to nsec */(1000000 * miliseconds);

    const decltype(t.tv_nsec) BILION = 1000000000;
    if (t.tv_nsec >= BILION)
    {
        t.tv_sec += 1;
        t.tv_nsec = t.tv_nsec - BILION;
    }

    int ret = pthread_cond_timedwait(&m_condition, m_mutex.getNativeHandle(), &t);

    if (ret == 0)
    {
        return ETimedWaitResult_Signal;
    }
    else if (ret == ETIMEDOUT)
    {
        return ETimedWaitResult_Timeout;
    }
    else if (ret == EPERM)
    {
        throw std::runtime_error("condition variable mutex in not locked");
    }
    else if (ret == EINVAL)
    {
        throw std::runtime_error("pthread_cond_timedwait reported invalid parameter");
    }
    else
    {
        throw std::runtime_error("unknown error occured in condition variable pthread_cond_timedwait");
    }
#endif
}

void ConditionVariable::signal()
{
#ifdef _WIN32
    #error not implemented
#else
    pthread_cond_signal(&m_condition);
#endif
}

void ConditionVariable::broadcast()
{
#ifdef _WIN32
    #error not implemented
#else
    pthread_cond_broadcast(&m_condition);
#endif
}

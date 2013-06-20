#include <sys/time.h>
#include <stdexcept>

#include "ConditionVariable.hpp"

using namespace Cake::Threading;

ConditionVariable::ConditionVariable(Mutex & mutex) : m_mutex(mutex)
{
    pthread_cond_init(&m_condition, 0);
}

ConditionVariable::~ConditionVariable()
{
    pthread_cond_destroy(&m_condition);
}

void ConditionVariable::wait()
{
    int ret = pthread_cond_wait(&m_condition, m_mutex.getNativeHandle());

    if (ret == EPERM)
    {
        throw std::runtime_error("condition variable mutex in not locked");
    }
}

ETimedWaitResult ConditionVariable::timedWait(unsigned seconds, unsigned miliseconds)
{
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
}

void ConditionVariable::signal()
{
    pthread_cond_signal(&m_condition);
}

void ConditionVariable::broadcast()
{
    pthread_cond_broadcast(&m_condition);
}

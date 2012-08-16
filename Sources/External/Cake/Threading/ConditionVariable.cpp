#include <sys/time.h>
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
    pthread_cond_wait(&m_condition, m_mutex.getNativeHandle());
}

void ConditionVariable::timedWait(unsigned seconds)
{
    timeval c;
    gettimeofday(&c, NULL);

    timespec t;
    t.tv_sec = c.tv_sec + seconds;
    t.tv_nsec = 0;

    pthread_cond_timedwait(&m_condition, m_mutex.getNativeHandle(), &t);
}

void ConditionVariable::signal()
{
    pthread_cond_signal(&m_condition);
}

void ConditionVariable::broadcast()
{
    pthread_cond_broadcast(&m_condition);
}

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

void ConditionVariable::timedWait(unsigned seconds, unsigned miliseconds)
{
    timeval c;
    gettimeofday(&c, NULL);

    timespec t;
    t.tv_sec = c.tv_sec + seconds;
    t.tv_nsec = /* to nsec */(c.tv_usec * 1000) + /* to nsec */(1000000 * miliseconds);

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

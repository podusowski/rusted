#include <sys/time.h>
#include "ConditionVariable.hpp"

using namespace Cake::Threading;

ConditionVariable::ConditionVariable()
{
    pthread_mutex_init(&m_mutex, 0);
    pthread_cond_init(&m_condition, 0);
}

ConditionVariable::~ConditionVariable()
{
    pthread_cond_destroy(&m_condition);
    pthread_mutex_destroy(&m_mutex);
}

void ConditionVariable::wait()
{
    pthread_mutex_lock(&m_mutex);
    pthread_cond_wait(&m_condition, &m_mutex);
}

void ConditionVariable::timedWait(unsigned seconds)
{
    timeval c;
    gettimeofday(&c, NULL);

    timespec t;
    t.tv_sec = c.tv_sec + seconds;
    t.tv_nsec = 0;

    pthread_mutex_lock(&m_mutex);
    pthread_cond_timedwait(&m_condition, &m_mutex, &t);
}

void ConditionVariable::signal()
{
    pthread_cond_signal(&m_condition);
}

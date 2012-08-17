#pragma once

#include <pthread.h>
#include "Mutex.hpp"

namespace Cake
{
namespace Threading
{

class ConditionVariable
{
public:
    ConditionVariable(Mutex &); 
    ~ConditionVariable();
    void wait();
    void timedWait(unsigned seconds, unsigned miliseconds = 0);
    void signal();
    void broadcast();

private:
    pthread_cond_t m_condition;
    Mutex & m_mutex;
};

}
}

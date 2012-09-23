#pragma once

#include <pthread.h>
#include "Mutex.hpp"

namespace Cake
{
namespace Threading
{

enum ETimedWaitResult
{
    ETimedWaitResult_Signal,
    ETimedWaitResult_Timeout
};

class ConditionVariable
{
public:
    ConditionVariable(Mutex &); 
    ~ConditionVariable();
    void wait();
    ETimedWaitResult timedWait(unsigned seconds, unsigned miliseconds = 0);
    void signal();
    void broadcast();

private:
    pthread_cond_t m_condition;
    Mutex & m_mutex;
};

}
}

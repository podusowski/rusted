#pragma once

// windows implementation from http://www.cs.wustl.edu/~schmidt/win32-cv-1.html

#ifdef _WIN32
    #include <windows.h>
#else
    #include <pthread.h>
#endif

#ifdef _WIN32
struct pthread_cond_t
{
    int waiters_count_;
    CRITICAL_SECTION waiters_count_lock_;
    HANDLE sema_;
    HANDLE waiters_done_;
    size_t was_broadcast_;
};
#endif

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
    typedef pthread_cond_t ConditionVariableHandle;

    ConditionVariable(Mutex &); 
    ~ConditionVariable();
    void wait();
    ETimedWaitResult timedWait(unsigned seconds, unsigned miliseconds = 0);
    void signal();
    void broadcast();

private:
    ConditionVariableHandle m_condition;
    Mutex & m_mutex;
};

}
}

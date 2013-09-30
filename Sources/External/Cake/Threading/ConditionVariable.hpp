#pragma once

#ifdef _WIN32
    #include <windows.h>
#else
    #include <pthread.h>
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

#ifdef _WIN32
    typedef HANDLE ConditionVariableHandle;
#else
    typedef pthread_cond_t ConditionVariableHandle;
#endif

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

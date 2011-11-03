#pragma once

#include <pthread.h>

namespace Cake
{
namespace Threading
{

class ConditionVariable
{
public:
    ConditionVariable(); 
    ~ConditionVariable();
    void wait();
    void signal();
    void broadcast();

private:
    pthread_cond_t m_condition;
    pthread_mutex_t m_mutex;
};

}
}

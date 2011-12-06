#pragma once

#include <pthread.h>

#include "ILock.hpp"

namespace Cake 
{
namespace Threading
{

class Mutex : public ILock
{
public:
    Mutex();
    void aquire();
    void release();

private:
    pthread_mutex_t m_mutex;
};

}
}

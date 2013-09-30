#pragma once

#ifdef _WIN32
    #include <windows.h>
#else
    #include <pthread.h>
#endif

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
    pthread_mutex_t * getNativeHandle();

private:
    pthread_mutex_t m_mutex;
};

}
}

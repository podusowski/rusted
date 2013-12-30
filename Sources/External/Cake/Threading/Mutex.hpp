#pragma once

#ifdef _WIN32
    #include <windows.h>
#else
    #include <pthread.h>
#endif

#ifdef _WIN32
typedef CRITICAL_SECTION pthread_mutex_t;
#endif

#include "ILock.hpp"

namespace Cake
{
namespace Threading
{

class Mutex : public ILock
{
public:
    typedef pthread_mutex_t MutexHandle;

    Mutex();
    void aquire();
    void release();
    MutexHandle * getNativeHandle();

private:
    MutexHandle m_mutex;
};

}
}

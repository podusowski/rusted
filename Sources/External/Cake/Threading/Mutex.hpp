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

#ifdef _WIN32
    typedef CRITICAL_SECTION MutexHandle;
#else
    typedef pthread_mutex_t MutexHandle;
#endif

    Mutex();
    void aquire();
    void release();
    MutexHandle * getNativeHandle();

private:
    MutexHandle m_mutex;
};

}
}

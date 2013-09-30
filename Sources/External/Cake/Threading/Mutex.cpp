#include <stdexcept>

#include "Mutex.hpp"

using namespace Cake::Threading;

Mutex::Mutex()
{
#ifdef _WIN32
    InitializeCriticalSection(&m_mutex);
#else
    int e = pthread_mutex_init(&m_mutex, 0);
    if (e)
    {
        throw std::runtime_error("error while initializing mutex");
    }
#endif
}

void Mutex::aquire()
{
#ifdef _WIN32
    EnterCriticalSection(&m_mutex);
#else
    pthread_mutex_lock(&m_mutex);
#endif
}

void Mutex::release()
{
#ifdef _WIN32
    LeaveCriticalSection(&m_mutex);
#else
    pthread_mutex_unlock(&m_mutex);
#endif
}

Mutex::MutexHandle * Mutex::getNativeHandle()
{
    return &m_mutex;
}

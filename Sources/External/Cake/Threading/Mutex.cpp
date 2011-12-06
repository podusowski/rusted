#include <stdexcept>

#include "Mutex.hpp"

using namespace Cake::Threading;

Mutex::Mutex()
{
    int e = pthread_mutex_init(&m_mutex, 0);
    if (e)
    {
        throw std::runtime_error("error while initializing mutex");
    }
}

void Mutex::aquire()
{
    pthread_mutex_lock(&m_mutex);
}

void Mutex::release()
{
    pthread_mutex_unlock(&m_mutex);
}

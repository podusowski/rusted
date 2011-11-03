#include "ScopedLock.hpp"

using namespace Cake::Threading;

ScopedLock::ScopedLock(ILock & lock) : m_lock(lock)
{
    m_lock.aquire();
}

ScopedLock::~ScopedLock()
{
    m_lock.release();
}

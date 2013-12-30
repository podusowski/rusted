#include <sys/time.h>
#include <stdexcept>

#include "ConditionVariable.hpp"

using namespace Cake::Threading;

ConditionVariable::ConditionVariable(Mutex & mutex) : m_mutex(mutex)
{
#ifdef _WIN32
    m_condition.waiters_count_ = 0;
    m_condition.was_broadcast_ = 0;
    m_condition.sema_ = CreateSemaphore (NULL,       // no security
                                  0,          // initially 0
                                  0x7fffffff, // max count
                                  NULL);      // unnamed 
    InitializeCriticalSection (&m_condition.waiters_count_lock_);
    m_condition.waiters_done_ = CreateEvent (NULL,  // no security
                                     FALSE, // auto-reset
                                     FALSE, // non-signaled initially
                                     NULL); // unnamedlse
#else
    pthread_cond_init(&m_condition, 0);
#endif
}

ConditionVariable::~ConditionVariable()
{
#ifdef _WIN32
    //TODO : cleanup
#else
    pthread_cond_destroy(&m_condition);
#endif
}

void ConditionVariable::wait()
{
#ifdef _WIN32
    EnterCriticalSection (&m_condition.waiters_count_lock_);
    m_condition.waiters_count_++;
    LeaveCriticalSection (&m_condition.waiters_count_lock_);

    SignalObjectAndWait (m_mutex.getNativeHandle(), m_condition.sema_, INFINITE, FALSE);
    EnterCriticalSection (&m_condition.waiters_count_lock_);

    m_condition.waiters_count_--;

    int last_waiter = m_condition.was_broadcast_ && m_condition.waiters_count_ == 0;

    LeaveCriticalSection (&m_condition.waiters_count_lock_);

    if (last_waiter)
        SignalObjectAndWait (m_condition.waiters_done_, m_mutex.getNativeHandle(), INFINITE, FALSE);
    else
        WaitForSingleObject (m_mutex.getNativeHandle(), INFINITE);
#else
    int ret = pthread_cond_wait(&m_condition, m_mutex.getNativeHandle());

    if (ret == EPERM)
    {
        throw std::runtime_error("condition variable mutex in not locked");
    }
#endif
}

ETimedWaitResult ConditionVariable::timedWait(unsigned seconds, unsigned miliseconds)
{
#ifdef _WIN32
    throw std::runtime_error("WIN32 is not supported");
#else
    timeval c;
    gettimeofday(&c, NULL);

    timespec t;
    t.tv_sec = c.tv_sec + seconds;
    t.tv_nsec = /* to nsec */(c.tv_usec * 1000) + /* to nsec */(1000000 * miliseconds);

    const decltype(t.tv_nsec) BILION = 1000000000;
    if (t.tv_nsec >= BILION)
    {
        t.tv_sec += 1;
        t.tv_nsec = t.tv_nsec - BILION;
    }

    int ret = pthread_cond_timedwait(&m_condition, m_mutex.getNativeHandle(), &t);

    if (ret == 0)
    {
        return ETimedWaitResult_Signal;
    }
    else if (ret == ETIMEDOUT)
    {
        return ETimedWaitResult_Timeout;
    }
    else if (ret == EPERM)
    {
        throw std::runtime_error("condition variable mutex in not locked");
    }
    else if (ret == EINVAL)
    {
        throw std::runtime_error("pthread_cond_timedwait reported invalid parameter");
    }
    else
    {
        throw std::runtime_error("unknown error occured in condition variable pthread_cond_timedwait");
    }
#endif
}

void ConditionVariable::signal()
{
#ifdef _WIN32

  EnterCriticalSection (&m_condition.waiters_count_lock_);
  int have_waiters = m_condition.waiters_count_ > 0;
  LeaveCriticalSection (&m_condition.waiters_count_lock_);

  // If there aren't any waiters, then this is a no-op.  
  if (have_waiters)
    ReleaseSemaphore (m_condition.sema_, 1, 0);

#else
    pthread_cond_signal(&m_condition);
#endif
}

void ConditionVariable::broadcast()
{
#ifdef _WIN32

  // This is needed to ensure that <waiters_count_> and <was_broadcast_> are
  // consistent relative to each other.
  EnterCriticalSection (&m_condition.waiters_count_lock_);
  int have_waiters = 0;

  if (m_condition.waiters_count_ > 0) {
    // We are broadcasting, even if there is just one waiter...
    // Record that we are broadcasting, which helps optimize
    // <pthread_cond_wait> for the non-broadcast case.
    m_condition.was_broadcast_ = 1;
    have_waiters = 1;
  }

  if (have_waiters) {
    // Wake up all the waiters atomically.
    ReleaseSemaphore (m_condition.sema_, m_condition.waiters_count_, 0);

    LeaveCriticalSection (&m_condition.waiters_count_lock_);

    // Wait for all the awakened threads to acquire the counting
    // semaphore. 
    WaitForSingleObject (m_condition.waiters_done_, INFINITE);
    // This assignment is okay, even without the <waiters_count_lock_> held 
    // because no other waiter threads can wake up to access it.
    m_condition.was_broadcast_ = 0;
  }
  else
    LeaveCriticalSection (&m_condition.waiters_count_lock_);



#else
    pthread_cond_broadcast(&m_condition);
#endif
}

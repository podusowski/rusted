#pragma once

#ifdef _WIN32
    #include <windows.h>
#else
    #include <pthread.h>
#endif

#include "IRunnable.hpp"
#include "Mutex.hpp"

namespace Cake
{
namespace Threading
{

class Thread
{
public:

#ifdef _WIN32
    typedef HANDLE ThreadHandle;
#else
    typedef pthread_t ThreadHandle;
#endif

    struct Context
    {
        Thread * thread;
        IRunnable * runnable;
    };

    Thread(IRunnable &);
    ~Thread();

    void start();
    void join();
    bool isRunning();

    static void wait(int secs, int mili = 0);
    static unsigned self();

private:
    static void * s_run(void *);

    bool m_isRunning;
    IRunnable & m_runnable;
    ThreadHandle m_thread;
    Mutex m_isRunningLock;
};

}
}


#pragma once

#include <stdexcept>
#include <errno.h>
#include <string.h> // strerror

#include "Threading/Mutex.hpp"
#include "Threading/ScopedLock.hpp"

namespace Cake 
{
namespace System
{

extern Threading::Mutex ATOMIC_SYSCALL_MUTEX;

class AtomicSyscallError : public std::runtime_error
{
public:
    AtomicSyscallError(const std::string & error) : 
        std::runtime_error(error + ", reason: " + strerror(errno))
    {
    }
};

}
}

#define ATOMIC_SYSCALL(system_call, return_variable, error_condition) \
    { \
        Cake::Threading::ScopedLock lock(Cake::System::ATOMIC_SYSCALL_MUTEX); \
        return_variable = system_call; \
        if (return_variable error_condition) \
        { \
            throw Cake::System::AtomicSyscallError(#system_call); \
        } \
    } \


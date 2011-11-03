#pragma once

#include "ILock.hpp"

namespace Cake 
{
namespace Threading
{

class ScopedLock
{
public:
    ScopedLock(ILock &);
    ~ScopedLock();

private:
    ILock & m_lock;
};

}
}

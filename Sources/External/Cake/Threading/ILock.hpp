#pragma once

namespace Cake 
{
namespace Threading
{

class ILock
{
public:
    virtual ~ILock() {}

    virtual void aquire() = 0;
    virtual void release() = 0;
};

}
}

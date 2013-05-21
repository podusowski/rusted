#pragma once

#include <cstring>

namespace Cake
{
namespace Networking
{
namespace Protocol
{

class IReadBuffer
{
public:
    virtual void read(void * destination, size_t size) = 0;
    virtual ~IReadBuffer() {}
};

}
}
}


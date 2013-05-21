#pragma once

#include <cstring>

namespace Cake
{
namespace Networking
{
namespace Protocol
{

class IWriteBuffer
{
public:
    virtual void write(const void * source, size_t size) = 0;
    virtual ~IWriteBuffer() {}
};

}
}
}


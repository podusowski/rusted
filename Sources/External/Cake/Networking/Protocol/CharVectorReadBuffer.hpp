#pragma once

#include <vector>

#include "IReadBuffer.hpp"

namespace Cake
{
namespace Networking
{
namespace Protocol
{

class CharVectorReadBuffer : public IReadBuffer
{
public:
    CharVectorReadBuffer(const std::vector<char> &);
    void read(void * destination, size_t size);

private:
    std::vector<char>::const_iterator m_rawBufferIterator;
};

}
}
}


#pragma once

#include <vector>

#include "IWriteBuffer.hpp"

namespace Cake
{
namespace Networking
{
namespace Protocol
{

class CharVectorWriteBuffer : public IWriteBuffer
{
public:
    CharVectorWriteBuffer(std::vector<char> &);
    void write(const void * source, size_t size);

private:
    std::vector<char> & m_buffer;
};

}
}
}


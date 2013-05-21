#include "CharVectorReadBuffer.hpp"

using namespace Cake::Networking::Protocol;

CharVectorReadBuffer::CharVectorReadBuffer(const std::vector<char> & data) :
    m_rawBufferIterator(data.begin())
{
}

void CharVectorReadBuffer::read(void * destination, size_t size)
{
    memcpy(destination, &(*m_rawBufferIterator), size);
    m_rawBufferIterator += size;
}


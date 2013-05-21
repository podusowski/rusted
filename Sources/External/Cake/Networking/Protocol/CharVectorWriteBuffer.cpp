#include "CharVectorWriteBuffer.hpp"

using namespace Cake::Networking::Protocol;

CharVectorWriteBuffer::CharVectorWriteBuffer(std::vector<char> & buffer) :
    m_buffer(buffer)
{
}

void CharVectorWriteBuffer::write(const void * source, size_t size)
{
    m_buffer.resize(m_buffer.size() + size);
    memcpy(&(m_buffer[m_buffer.size() - size]), source, size);
}


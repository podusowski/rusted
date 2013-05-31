#include "CakeWriteBuffer.hpp"

using namespace Cake::Networking::Protocol;

CakeWriteBuffer::CakeWriteBuffer(Cake::Networking::Socket & socket) :
    m_socket(socket)
{
}

void CakeWriteBuffer::write(const void * source, size_t size)
{
    m_socket.stream().send(source, size);
}


#include "CakeWriteBuffer.hpp"

using namespace Common::RustedCodec;

CakeWriteBuffer::CakeWriteBuffer(Cake::Networking::Socket & socket) :
    m_socket(socket)
{
}

void CakeWriteBuffer::write(const void * source, size_t size)
{
    m_socket.stream().send(source, size);
}

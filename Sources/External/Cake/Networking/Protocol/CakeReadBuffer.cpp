#include "CakeReadBuffer.hpp"

using namespace Cake::Networking::Protocol;

CakeReadBuffer::CakeReadBuffer(Cake::Networking::Socket & socket) :
    m_socket(socket)
{
}

void CakeReadBuffer::read(void * destination, size_t size)
{
    m_socket.stream().receive(destination, size);
}

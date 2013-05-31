#pragma once

#include "Cake/Networking/Socket.hpp"
#include "IWriteBuffer.hpp"

namespace Cake
{
namespace Networking
{
namespace Protocol
{

class CakeWriteBuffer : public IWriteBuffer
{
public:
    CakeWriteBuffer(Cake::Networking::Socket & socket);
    void write(const void * source, size_t size);

private:
    Cake::Networking::Socket & m_socket;
};

}
}
}


#pragma once

#include <cstddef>

#include "Cake/Networking/Socket.hpp"
#include "IReadBuffer.hpp"

namespace Cake
{
namespace Networking
{
namespace Protocol
{

class CakeReadBuffer : public IReadBuffer
{
public:
    CakeReadBuffer(Cake::Networking::Socket & socket);
    void read(void *, size_t);

private:
    Cake::Networking::Socket & m_socket;
};

}
}
}


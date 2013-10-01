#pragma once

#include <vector>
#include <memory>

#include "Socket.hpp"
#include "ServerSocket.hpp"

namespace Cake
{
namespace Networking
{

class ServerSocketPool
{
public:
    typedef std::pair<std::shared_ptr<Socket>, std::shared_ptr<ServerSocket>> AcceptReturnType;

    void add(std::shared_ptr<ServerSocket> server);
    AcceptReturnType accept();

private:
    std::vector<std::shared_ptr<ServerSocket>> m_servers;
};

}
}


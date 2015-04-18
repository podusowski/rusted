#pragma once

#include "Socket.hpp"
#include "ServerSocket.hpp"

#include <functional>

namespace Cake
{
namespace Networking
{

class MainLoop
{
public:
    using Servers = std::vector<std::shared_ptr<ServerSocket>>;
    using SocketConnected = std::function<void(ServerSocket &, std::shared_ptr<Socket>)>;

    explicit MainLoop(Servers servers);

    void run(SocketConnected);

private:
    void wait(SocketConnected);

    Servers m_servers;
};

}
}

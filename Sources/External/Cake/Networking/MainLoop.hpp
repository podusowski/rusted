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

    explicit MainLoop(Servers servers);
    void run();

private:
    void wait();

    Servers m_servers;
};

}
}

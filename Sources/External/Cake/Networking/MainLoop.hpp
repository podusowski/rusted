#pragma once

#include "Socket.hpp"
#include "Acceptor.hpp"

#include <vector>
#include <functional>

namespace Cake
{
namespace Networking
{

class MainLoop
{
public:
    using Servers = std::vector<std::shared_ptr<Acceptor>>;

    explicit MainLoop(Servers servers);
    void run();

private:
    void wait();

    Servers m_servers;
};

}
}

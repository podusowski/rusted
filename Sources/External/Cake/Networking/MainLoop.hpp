#pragma once

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
    using Selectables = std::vector<std::shared_ptr<ISelectable>>;

    explicit MainLoop(Selectables servers);
    void run();

private:
    void wait();

    Selectables m_servers;
};

}
}

#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>

#include "Socket.hpp"
#include "ServerSocket.hpp"

namespace Cake
{
namespace Networking
{

class ServerSocketPool
{
public:
    typedef std::pair<boost::shared_ptr<Socket>, boost::shared_ptr<ServerSocket>> AcceptReturnType;

    void add(boost::shared_ptr<ServerSocket> server);
    AcceptReturnType accept();

private:
    std::vector<boost::shared_ptr<ServerSocket>> m_servers;
};

}
}


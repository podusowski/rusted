#pragma once

#include <boost/shared_ptr.hpp>

#include "Socket.hpp"

namespace Cake 
{
namespace Networking
{

class ServerSocket
{
public:
    static boost::shared_ptr<ServerSocket> createTcpServer(unsigned port);
    static boost::shared_ptr<ServerSocket> createUnixServer(const std::string & path);

    boost::shared_ptr<Socket> accept();

private:

    ServerSocket(int sockFd);
    int m_sockFd;
};

}
}

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
    static std::shared_ptr<ServerSocket> createTcpServer(unsigned port);
    static std::shared_ptr<ServerSocket> createUnixServer(const std::string & path);

    std::shared_ptr<Socket> accept();
    int getNativeHandle() const;

private:
    ServerSocket(int sockFd);
    static int createDescriptor(int family);
    static void listen(int descriptor);
    int m_sockFd;
};

}
}

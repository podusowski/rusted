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
    using ClientConnected = std::function<void(std::shared_ptr<Socket>)>;

    static auto createTcpServer(unsigned port,
                                ClientConnected = ClientConnected{}) -> std::shared_ptr<ServerSocket>;

    static auto createUnixServer(const std::string & path,
                                 ClientConnected = ClientConnected{}) -> std::shared_ptr<ServerSocket>;

    auto accept() -> std::shared_ptr<Socket>;
    void act();
    int getNativeHandle() const;

private:
    ServerSocket(int sockFd, ClientConnected = ClientConnected{});
    static int createDescriptor(int family);
    static void listen(int descriptor);
    int m_sockFd;
    ClientConnected m_clientConnected;
};

}
}

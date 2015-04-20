#pragma once

#include "Socket.hpp"
#include "ISelectable.hpp"

namespace Cake
{
namespace Networking
{

class Acceptor : public ISelectable
{
public:
    using ClientConnected = std::function<void(std::shared_ptr<Socket>)>;

    static auto createTcpServer(unsigned port,
                                ClientConnected = ClientConnected{}) -> std::shared_ptr<Acceptor>;

    static auto createUnixServer(const std::string & path,
                                 ClientConnected = ClientConnected{}) -> std::shared_ptr<Acceptor>;

    auto accept() -> std::shared_ptr<Socket>;

    // ISelectable
    void act() override;
    auto nativeHandle() -> int const override;

private:
    Acceptor(int sockFd, ClientConnected = ClientConnected{});
    static int createDescriptor(int family);
    static void listen(int descriptor);
    int m_sockFd;
    ClientConnected m_clientConnected;
};

}
}

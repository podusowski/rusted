#pragma once

#include "Common/Messages/Messages.hpp"
#include "Server/Network/IConnectionListener.hpp"

namespace Server
{

template <typename ServiceType>
class AbstractService : public Server::Network::IConnectionListener
{
public:
    // NOTE: ServiceType isn't constructed yet, but HandlerCaller is lazy, it doesn't do anything
    //       with service until message arrive
    AbstractService() : m_caller(reinterpret_cast<ServiceType&>(*this)) {}

    virtual void messageReceived(Server::Network::IConnection & connection, Common::Messages::AbstractMessage & message)
    {
        m_caller.call(message, connection);
    }

    virtual void handle(const Common::Messages::AbstractMessage &, Server::Network::IConnection &) {}

private:
    Common::Messages::HandlerCaller1<ServiceType, Server::Network::IConnection &> m_caller;
};

}

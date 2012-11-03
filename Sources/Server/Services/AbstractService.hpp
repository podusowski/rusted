#pragma once

#include "Cake/Diagnostics/Logger.hpp"

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
    AbstractService() : m_caller(static_cast<ServiceType&>(*this)) {}

    virtual void messageReceived(Server::Network::IConnection & connection, Common::Messages::AbstractMessage & message)
    {
        try
        {
            m_caller.call(message, connection);
        }
        catch (std::exception & ex)
        {
            LOG_WARN << "Exception while handling the message:" << ex.what();

            Common::Messages::Exception exception;
            exception.description = ex.what();
            connection.send(exception);

            throw ex;
        }
    }

    virtual void handle(const Common::Messages::AbstractMessage &, Server::Network::IConnection &) {}

private:
    Common::Messages::HandlerCaller1<ServiceType, Server::Network::IConnection &> m_caller;
};

}

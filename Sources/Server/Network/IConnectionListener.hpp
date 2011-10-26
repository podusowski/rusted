#pragma once

#include <Common/Messages/Messages.hpp>
#include <Network/IConnection.hpp>

namespace Server
{

namespace Network
{

class IConnection;

class IConnectionListener
{
public:
    virtual void messageReceived(Server::Network::IConnection & connection,
                                 Common::Messages::AbstractMessage & message) = 0;
};

}

}

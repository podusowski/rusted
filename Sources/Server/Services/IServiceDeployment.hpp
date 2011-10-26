#pragma once

#include "Server/Network/IConnection.hpp"

namespace Server
{
namespace Services
{

class IServiceDeployment
{
public:
    virtual void deployNewConnection(Server::Network::IConnection &) = 0;
    virtual void deployAuthorizedConnection(Server::Network::IConnection &) = 0;

    virtual ~IServiceDeployment() {}
};

}
}

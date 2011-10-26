#pragma once

#include "Common/Messages/Messages.hpp"
#include "Server/Services/AbstractService.hpp"

namespace Server
{
namespace Services 
{

class ServerInfoService: public Server::AbstractService<ServerInfoService>
{
public:
    void handle(const Common::Messages::ServerVersionReq &, Network::IConnection &);
    void handle(const Common::Messages::AbstractMessage &, Network::IConnection &) {}
};

}
}

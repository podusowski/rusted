#pragma once

#include "Server/Services/AbstractService.hpp"
#include "Common/Game/Universe.hpp"

namespace Server
{
namespace Services 
{

class PlayerService : public Server::AbstractService<PlayerService>
{
public:
    PlayerService(Common::Game::Universe & universe);
    void handle(const Common::Messages::PlayerResourcesStatusReq &, Network::IConnection &);
    void handle(const Common::Messages::PlayerEntitiesStatusReq &, Network::IConnection &);
    void handle(const Common::Messages::AbstractMessage &, Network::IConnection &) {}

private:
    Common::Game::Universe & m_universe;
};

}
}


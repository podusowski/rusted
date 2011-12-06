#pragma once

#include "Server/Services/AbstractService.hpp"
#include "Common/Game/EntityContainer.hpp"

namespace Server
{
namespace Services 
{

class PlayerService : public Server::AbstractService<PlayerService>
{
public:
    PlayerService(Common::Game::EntityContainer & entityContainer);
    void handle(const Common::Messages::PlayerResourcesStatusReq &, Network::IConnection &);
    void handle(const Common::Messages::PlayerEntitiesStatusReq &, Network::IConnection &);
    void handle(const Common::Messages::AbstractMessage &, Network::IConnection &) {}

private:
    Common::Game::EntityContainer & m_entityContainer;
};

}
}


#pragma once

#include "Common/Game/Universe.hpp"

#include "Services/AbstractService.hpp"
#include "Game/PlayerContainer.hpp"

namespace Server
{
namespace Services
{

class EntityService : public Server::AbstractService<EntityService>
{
public:
    EntityService(Common::Game::Universe &, Game::PlayerContainer &);
    void handle(const Common::Messages::EntityGetInfoReq &, Network::IConnection &);
    void handle(const Common::Messages::EntityChangeCourseReq &, Network::IConnection &);
    void handle(const Common::Messages::AbstractMessage &, Network::IConnection &) {}

private:
    Common::Game::Universe & m_universe;
    Game::PlayerContainer & m_playerContainer;
};

}
}

#pragma once

#include "Common/Game/Universe.hpp"
#include "Common/Game/Object/Ship.hpp"

#include "Game/Actions/ActionBuilder.hpp"
#include "Game/PlayerContainer.hpp"
#include "Utils.hpp"

#include "Services/AbstractService.hpp"

namespace Server
{
namespace Services
{

class EntityService : public Server::AbstractService<EntityService>
{
public:
    EntityService(Common::Game::Universe &, Game::PlayerContainer &);

    void handle(const Common::Messages::EntityChangeCourseReq &, Network::IConnection &);
    void handle(const Common::Messages::GetVisibleObjects &, Network::IConnection &);
    void handle(const Common::Messages::GetObjectInfo &, Network::IConnection &);
    void handle(const Common::Messages::SelectObject &, Network::IConnection &);
    void handle(const Common::Messages::FocusObject &, Network::IConnection &);
    void handle(const Common::Messages::FetchAvailableActions &, Network::IConnection &);
    void handle(const Common::Messages::ExecuteAction &, Network::IConnection &);

    void handle(const Common::Messages::AbstractMessage &, Network::IConnection &) {}

private:
    Common::Game::Universe & m_universe;
    Game::PlayerContainer & m_playerContainer;
    Server::Game::Actions::ActionBuilder m_actionBuilder;
    Utils m_utils;
};

}
}

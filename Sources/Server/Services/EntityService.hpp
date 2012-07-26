#pragma once

#include "Common/Game/Universe.hpp"

#include "Services/AbstractService.hpp"
#include "Game/PlayerContainer.hpp"
#include "Common/Game/Object/Ship.hpp"

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
    void handle(const Common::Messages::GetVisibleObjects &, Network::IConnection &);
    void handle(const Common::Messages::GetObjectInfo &, Network::IConnection &);
    void handle(const Common::Messages::AttackObject &, Network::IConnection &);
    void handle(const Common::Messages::SelectObject &, Network::IConnection &);
    void handle(const Common::Messages::FocusObject &, Network::IConnection &);
    void handle(const Common::Messages::FetchAvailableActions &, Network::IConnection &);
    void handle(const Common::Messages::ExecuteAction &, Network::IConnection &);

    void handle(const Common::Messages::AbstractMessage &, Network::IConnection &) {}

private:
    void sendShipInfo(Common::Game::Object::Ship &, Network::IConnection &);

    Common::Game::Universe & m_universe;
    Game::PlayerContainer & m_playerContainer;
};

}
}

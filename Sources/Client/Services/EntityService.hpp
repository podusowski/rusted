#pragma once

#include <set>
#include <boost/optional.hpp>
#include <boost/function.hpp>

#include "Cake/DependencyInjection/Inject.hpp"

#include "Common/Game/IRustedTime.hpp"
#include "Common/Game/Universe.hpp"
#include "Common/Game/Object/Ship.hpp"

#include "Client/Network/IConnectionListener.hpp"
#include "Client/Network/IConnection.hpp"
#include "Client/Game/PlayerInfo.hpp"
#include "Client/Services/AbstractService.hpp"

namespace Client
{
namespace Services
{

class EntityService : public AbstractService<EntityService> 
{
public:
    typedef boost::function<void()> MyEntitiesFetchedCallback;

    EntityService(Network::IConnection &, Game::PlayerInfo &, Common::Game::Universe &);

    void fetchMyEntitiesInfo(MyEntitiesFetchedCallback);
    void setCurrentEntity(Common::Game::Object::Ship &);
    Common::Game::Object::Ship & getCurrentEntity();
    void setCourse(Common::Game::Position);

    void handle(const Common::Messages::PlayerEntitiesStatusResp &);
    void handle(const Common::Messages::ShipInfo &);
    void handle(const Common::Messages::AbstractMessage &) {}

private:
    Client::Game::PlayerInfo & m_playerInfo;
    Client::Network::IConnection & m_connection;
    Cake::DependencyInjection::Inject<Common::Game::IRustedTime> m_time;
    Common::Game::Universe & m_universe;

    std::set<int> m_myEntities;
    boost::optional<Common::Game::Object::Ship *> m_currentShip;

    MyEntitiesFetchedCallback m_myEntitiesFetchedCallback;
};

}
}

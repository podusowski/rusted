#pragma once

#include <set>
#include <boost/optional.hpp>
#include <boost/function.hpp>

#include "Common/Game/IRustedTime.hpp"

#include "Client/Network/IConnectionListener.hpp"
#include "Client/Network/Connection.hpp"
#include "Client/Game/EntityContainer.hpp"
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

    EntityService(Network::Connection &, Common::Game::IRustedTime &, Game::PlayerInfo &);

    void fetchMyEntitiesInfo(MyEntitiesFetchedCallback);
    void setCurrentEntity(Common::Game::Entity &);
    Common::Game::Entity & getCurrentEntity();
    void setCourse(Common::Game::Entity::Position);

    Client::Game::EntityContainer & getEntityContainer();

    void handle(const Common::Messages::PlayerEntitiesStatusResp &);
    void handle(const Common::Messages::EntityGetInfoResp &);
    void handle(const Common::Messages::AbstractMessage &) {}

private:
    Client::Game::PlayerInfo & m_playerInfo;
    Client::Game::EntityContainer m_entityContainer;
    Client::Network::Connection & m_connection;
    Common::Game::IRustedTime & m_time;

    std::set<int> m_myEntities;
    boost::optional<Common::Game::Entity *> m_currentEntity;

    MyEntitiesFetchedCallback m_myEntitiesFetchedCallback;
};

}
}

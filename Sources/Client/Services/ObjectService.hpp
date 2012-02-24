#pragma once

#include <set>

#include "Common/Game/Universe.hpp"
#include "Services/AbstractService.hpp"
#include "Network/IConnection.hpp"

namespace Client
{
namespace Services
{

class ObjectService : public AbstractService<ObjectService> 
{
public:
    typedef boost::function<void()> PlayerShipsFetchedCallback;

    ObjectService(Network::IConnection &, Common::Game::Universe &);

    void fetchVisibleObjects();
    void fetchPlayerShips(PlayerShipsFetchedCallback);

    void handle(const Common::Messages::VisibleObjects &);
    void handle(const Common::Messages::PlayerEntitiesStatusResp &);
    void handle(const Common::Messages::ShipInfo &);
    void handle(const Common::Messages::StaticObjectInfoResp &);
    void handle(const Common::Messages::EntityChangeCourseReq &);

    void handle(const Common::Messages::AbstractMessage &) {}

private:
    void tryCallPlayerShipsFetchedCallback(int shipId);

    Network::IConnection & m_connection;
    Common::Game::Universe & m_universe;
    std::set<int> m_playerShips;
    PlayerShipsFetchedCallback m_playerShipsFetchedCallback;
};

}
}

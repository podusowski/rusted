#pragma once

#include <set>
#include <map>
#include <memory>

#include <boost/signals2.hpp>

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
    typedef boost::signals2::signal<void(unsigned /* id */, const std::string & /* name */)> PlayerNameSignal;

    ObjectService(Network::IConnection &, Common::Game::Universe &);

    void fetchVisibleObjects();
    void fetchPlayerShips(PlayerShipsFetchedCallback);
    boost::signals2::connection fetchPlayerName(int id, PlayerNameSignal::slot_type);

    void handle(const Common::Messages::VisibleObjects &);
    void handle(const Common::Messages::PlayerShips &);
    void handle(const Common::Messages::ShipInfo &);
    void handle(const Common::Messages::AsteroidInfo &);
    void handle(const Common::Messages::ShipCourseInfo &);
    void handle(const Common::Messages::ObjectCargoInfo &);
    void handle(const Common::Messages::PlayerName &);

    void handle(const Common::Messages::AbstractMessage &) {}

private:
    void tryCallPlayerShipsFetchedCallback(int shipId);

    Network::IConnection & m_connection;
    Common::Game::Universe & m_universe;
    std::set<int> m_playerShips;
    std::map<int, std::shared_ptr<PlayerNameSignal>> m_playerNameSignals;
    PlayerShipsFetchedCallback m_playerShipsFetchedCallback;
};

}
}

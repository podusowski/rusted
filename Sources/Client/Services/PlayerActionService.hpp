#pragma once

#include <boost/optional.hpp>

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

class PlayerActionService : public AbstractService<PlayerActionService> 
{
public:
    PlayerActionService(Network::IConnection &, Game::PlayerInfo &, Common::Game::Universe &);

    void setCurrentEntity(Common::Game::Object::Ship &);
    Common::Game::Object::Ship & getCurrentEntity();
    void setCourse(Common::Game::Position);

    void handle(const Common::Messages::AbstractMessage &) {}

private:
    Client::Game::PlayerInfo & m_playerInfo;
    Client::Network::IConnection & m_connection;
    Cake::DependencyInjection::Inject<Common::Game::IRustedTime> m_time;
    Common::Game::Universe & m_universe;

    boost::optional<Common::Game::Object::Ship *> m_currentShip;
};

}
}

#pragma once

#include <set>
#include <boost/shared_ptr.hpp>

#include "Server/Game/Actions/IAction.hpp"
#include "Server/Network/IConnection.hpp"
#include "Server/Game/IPlayerContainer.hpp"
#include "Server/Game/IShipClassContainer.hpp"
#include "Common/Game/Universe.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "IActionFactory.hpp"
#include "ActionDescription.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class ActionFactory : public IActionFactory
{
public:
    ActionFactory(Common::Game::Universe &, Server::Game::IPlayerContainer &);

    std::shared_ptr<Server::Game::Actions::IAction> create(
        Server::Network::IConnection & connection,
        Common::Game::IPlayer &,
        const ActionParameters &);

    ActionDescription getActionDescription(unsigned id, unsigned parameter);

private:
    Cake::DependencyInjection::Inject<Common::Game::IRustedTime> m_time;
    Cake::DependencyInjection::Inject<Game::IShipClassContainer> m_shipClassContainer;
    Common::Game::Universe & m_universe;
    Server::Game::IPlayerContainer & m_playerContainer;
};

}
}
}


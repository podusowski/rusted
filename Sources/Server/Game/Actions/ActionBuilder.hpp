#pragma once

#include <set>
#include <boost/shared_ptr.hpp>

#include "Server/Game/Actions/IAction.hpp"
#include "Server/Network/IConnection.hpp"
#include "Server/Game/IPlayerContainer.hpp"
#include "Common/Game/Universe.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "IActionFactory.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class ActionBuilder : public IActionFactory
{
public:
    ActionBuilder(Common::Game::Universe &, Server::Game::IPlayerContainer &);

    boost::shared_ptr<Server::Game::Actions::IAction> build(
        Server::Network::IConnection & connection,
        Server::Game::IPlayer &,
        unsigned id);

private:
    Cake::DependencyInjection::Inject<Common::Game::IRustedTime> m_time;
    Common::Game::Universe & m_universe;
    Server::Game::IPlayerContainer & m_playerContainer;
};

}
}
}


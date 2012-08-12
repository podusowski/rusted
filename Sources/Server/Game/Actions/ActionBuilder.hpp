#pragma once

#include <boost/shared_ptr.hpp>

#include "Server/Game/Actions/IAction.hpp"
#include "Server/Network/IConnection.hpp"
#include "Server/Game/IPlayerContainer.hpp"
#include "Common/Game/Universe.hpp"
#include "Common/Game/Object/Ship.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class ActionBuilder
{
public:
    boost::shared_ptr<Server::Game::Actions::IAction> build(
        Server::Network::IConnection & connection,
        Server::Game::IPlayerContainer &,
        Server::Game::IPlayer &,
        Common::Game::Universe &,
        unsigned id);
};

}
}
}


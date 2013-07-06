#pragma once

#include <boost/shared_ptr.hpp>

#include "Server/Game/Actions/IAction.hpp"
#include "Server/Network/IConnection.hpp"
#include "Server/Game/IPlayerContainer.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class IActionFactory 
{
public:
    virtual ~IActionFactory() {}

    virtual boost::shared_ptr<Server::Game::Actions::IAction> create(
        Server::Network::IConnection &,
        Common::Game::IPlayer &,
        unsigned id,
        unsigned parameter,
        Common::Game::Object::ObjectBase::StrictId focusedObjectId,
        Common::Game::Object::ObjectBase::Id selectedObjectId) = 0;
};

}
}
}


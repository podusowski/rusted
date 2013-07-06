#pragma once

#include <gmock/gmock.h>

#include "Game/Actions/IActionFactory.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class ActionFactoryMock : public IActionFactory
{
public:
    MOCK_METHOD6(create, boost::shared_ptr<Server::Game::Actions::IAction>(
                            Server::Network::IConnection &,
                            Common::Game::IPlayer &,
                            unsigned id,
                            unsigned parameter,
                            Common::Game::Object::ObjectBase::StrictId focusedObjectId,
                            Common::Game::Object::ObjectBase::Id selectedObjectId));
};

}
}
}


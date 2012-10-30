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
    MOCK_METHOD4(create, boost::shared_ptr<Server::Game::Actions::IAction>(
                            Server::Network::IConnection &,
                            Server::Game::IPlayer &,
                            unsigned id,
                            unsigned parameter));
};

}
}
}


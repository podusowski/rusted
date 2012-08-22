#include <gtest/gtest.h>

#include "Server/UnitTests/AbstractTest.hpp"

#include "Common/Game/UnitTests/RustedTimeStub.hpp"
#include "Server/Game/Actions/ActionFactory.hpp"
#include "Server/Network/UnitTests/ConnectionMock.hpp"
#include "Server/Game/UnitTests/PlayerContainerMock.hpp"
#include "Server/Game/UnitTests/PlayerMock.hpp"
#include "Common/Game/Object/UnitTests/ShipMock.hpp"
#include "Server/Game/Actions/Attack.hpp"

using namespace testing;

class ActionFactoryTest : public Server::AbstractTest
{
};

TEST_F(ActionFactoryTest, BuildAttack)
{
    const int ATTACK_ID = 1;

    Server::Network::ConnectionMock connection;
    Server::Game::PlayerContainerMock playerContainer;
    Server::Game::PlayerMock player;
    Common::Game::Universe universe;
    Common::Game::Object::ShipMock ship1;
    Common::Game::Object::ShipMock ship2;

    ON_CALL(player, getFocusedObject()).WillByDefault(ReturnRef(ship1));
    ON_CALL(player, getSelectedObject()).WillByDefault(ReturnRef(ship2));

    Server::Game::Actions::ActionFactory factory(universe, playerContainer);
    auto attackAction = factory.create(connection, player, ATTACK_ID);

    ASSERT_EQ(typeid(Server::Game::Actions::Attack), typeid(*attackAction));
}


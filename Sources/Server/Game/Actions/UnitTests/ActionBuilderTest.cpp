#include <gtest/gtest.h>

#include "Server/Game/Actions/ActionBuilder.hpp"
#include "Server/Network/UnitTests/ConnectionMock.hpp"
#include "Server/Game/UnitTests/PlayerContainerMock.hpp"
#include "Common/Game/Object/UnitTests/ShipMock.hpp"
#include "Server/Game/Actions/Attack.hpp"

TEST(ActionBuilderTest, BuildAttack)
{
    const int ATTACK_ID = 1;

    Server::Network::ConnectionMock connection;
    Server::Game::PlayerContainerMock playerContainer;
    Common::Game::Universe universe;
    Common::Game::Object::ShipMock ship1;
    Common::Game::Object::ShipMock ship2;

    Server::Game::Actions::ActionBuilder builder;
    auto attackAction = builder.build(connection, playerContainer, universe, ATTACK_ID, ship1, &ship2);

    ASSERT_EQ(typeid(Server::Game::Actions::Attack), typeid(*attackAction));
}

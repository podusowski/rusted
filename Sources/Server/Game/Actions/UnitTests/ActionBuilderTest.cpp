#include <gtest/gtest.h>

#include "Server/UnitTests/AbstractTest.hpp"

#include "Common/Game/UnitTests/RustedTimeStub.hpp"
#include "Server/Game/Actions/ActionBuilder.hpp"
#include "Server/Network/UnitTests/ConnectionMock.hpp"
#include "Server/Game/UnitTests/PlayerContainerMock.hpp"
#include "Server/Game/UnitTests/PlayerMock.hpp"
#include "Common/Game/Object/UnitTests/ShipMock.hpp"
#include "Server/Game/Actions/Attack.hpp"

using namespace testing;

class ActionBuilderTest : public Server::AbstractTest
{
};

TEST_F(ActionBuilderTest, BuildAttack)
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

    Server::Game::Actions::ActionBuilder builder(universe, playerContainer);
    auto attackAction = builder.build(connection, player, ATTACK_ID);

    ASSERT_EQ(typeid(Server::Game::Actions::Attack), typeid(*attackAction));
}

TEST_F(ActionBuilderTest, GlobalCooldown)
{
    const int ATTACK_ID = 1;
    const int PLAYER_ID = 2;

    Server::Network::ConnectionMock connection;
    Server::Game::PlayerContainerMock playerContainer;
    Server::Game::PlayerMock player;
    Common::Game::Universe universe;
    Common::Game::Object::ShipMock ship1;
    Common::Game::Object::ShipMock ship2;

    ON_CALL(player, getFocusedObject()).WillByDefault(ReturnRef(ship1));
    ON_CALL(player, getSelectedObject()).WillByDefault(ReturnRef(ship2));
    ON_CALL(player, getId()).WillByDefault(Return(PLAYER_ID));

    boost::function<void()> timerCallback;
    EXPECT_CALL(getTimeMock(), createTimer(_, _)).Times(2).WillRepeatedly(SaveArg<1>(&timerCallback));

    EXPECT_CALL(playerContainer, getConnectionById(PLAYER_ID)).Times(1).WillOnce(ReturnRef(connection));

    Server::Game::Actions::ActionBuilder builder(universe, playerContainer);
    builder.build(connection, player, ATTACK_ID);

    // global cooldown timer is not expired yet
    EXPECT_ANY_THROW(builder.build(connection, player, ATTACK_ID));

    timerCallback();

    // global cooldown has passed so we can do some action again
    builder.build(connection, player, ATTACK_ID);
}


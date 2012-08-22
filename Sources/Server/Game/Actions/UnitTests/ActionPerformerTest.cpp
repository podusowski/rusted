#include <gtest/gtest.h>

#include "Server/UnitTests/AbstractTest.hpp"

#include "Server/Game/Actions/ActionPerformer.hpp"
#include "Server/Network/UnitTests/ConnectionMock.hpp"
#include "Server/Game/UnitTests/PlayerContainerMock.hpp"
#include "Server/Game/UnitTests/PlayerMock.hpp"
#include "Common/Game/Object/UnitTests/ShipMock.hpp"
#include "Server/Game/Actions/Attack.hpp"
#include "ActionFactoryMock.hpp"
#include "ActionMock.hpp"

using namespace testing;

class ActionPerformerTest : public Server::AbstractTest
{
};

TEST_F(ActionPerformerTest, Perform)
{
    const int ATTACK_ID = 1;
    const int PLAYER_ID = 2;

    Server::Network::ConnectionMock connection;
    Server::Game::PlayerContainerMock playerContainer;
    Server::Game::PlayerMock player;
    Server::Game::Actions::ActionFactoryMock actionFactory;
    boost::shared_ptr<Server::Game::Actions::IAction> action(new Server::Game::Actions::ActionMock);
    Common::Game::Universe universe;
    Common::Game::Object::ShipMock ship1;
    Common::Game::Object::ShipMock ship2;

    ON_CALL(player, getFocusedObject()).WillByDefault(ReturnRef(ship1));
    ON_CALL(player, getSelectedObject()).WillByDefault(ReturnRef(ship2));
    ON_CALL(player, getId()).WillByDefault(Return(PLAYER_ID));

    boost::function<void()> timerCallback;
    EXPECT_CALL(getTimeMock(), createTimer(_, _)).Times(1).WillRepeatedly(SaveArg<1>(&timerCallback));
    EXPECT_CALL(actionFactory, build(_, _, ATTACK_ID)).Times(1).WillRepeatedly(Return(action));

    EXPECT_CALL(dynamic_cast<Server::Game::Actions::ActionMock&>(*action), execute()).Times(1);

    Server::Game::Actions::ActionPerformer performer(actionFactory, universe, playerContainer);
    performer.perform(connection, player, ATTACK_ID);

    // hack for gmock bug: http://code.google.com/p/googlemock/issues/detail?id=79
    Mock::VerifyAndClear(&actionFactory);
}

/*
TEST_F(ActionPerformerTest, GlobalCooldown)
{
    const int ATTACK_ID = 1;
    const int PLAYER_ID = 2;

    Server::Network::ConnectionMock connection;
    Server::Game::PlayerContainerMock playerContainer;
    Server::Game::PlayerMock player;
    Server::Game::Actions::ActionFactoryMock actionFactory;
    boost::shared_ptr<Server::Game::Actions::IAction> action(new Server::Game::Actions::ActionMock);
    Common::Game::Universe universe;
    Common::Game::Object::ShipMock ship1;
    Common::Game::Object::ShipMock ship2;

    ON_CALL(player, getFocusedObject()).WillByDefault(ReturnRef(ship1));
    ON_CALL(player, getSelectedObject()).WillByDefault(ReturnRef(ship2));
    ON_CALL(player, getId()).WillByDefault(Return(PLAYER_ID));

    boost::function<void()> timerCallback;
    EXPECT_CALL(getTimeMock(), createTimer(_, _)).Times(2).WillRepeatedly(SaveArg<1>(&timerCallback));
    EXPECT_CALL(playerContainer, getConnectionById(PLAYER_ID)).Times(1).WillOnce(ReturnRef(connection));
    EXPECT_CALL(actionFactory, build(_, _, ATTACK_ID)).Times(2).WillRepeatedly(Return(action));

    Server::Game::Actions::ActionPerformer performer(actionFactory, universe, playerContainer);
    performer.perform(connection, player, ATTACK_ID);

    // global cooldown timer is not expired yet
    EXPECT_ANY_THROW(performer.perform(connection, player, ATTACK_ID));

    timerCallback();

    // global cooldown has passed so we can do some action again without exception
    performer.perform(connection, player, ATTACK_ID);

    // hack for gmock bug: http://code.google.com/p/googlemock/issues/detail?id=79
    Mock::VerifyAndClear(&actionFactory);
}
*/

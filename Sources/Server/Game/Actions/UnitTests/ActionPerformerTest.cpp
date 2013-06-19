#include <gtest/gtest.h>

#include "Server/UnitTests/AbstractTest.hpp"

#include "Server/Game/Actions/ActionPerformer.hpp"
#include "Server/Network/UnitTests/ConnectionMock.hpp"
#include "Server/Game/UnitTests/PlayerContainerMock.hpp"
#include "Common/Game/UnitTests/PlayerMock.hpp"
#include "Common/Game/Object/UnitTests/ShipMock.hpp"
#include "Server/Game/Actions/Attack.hpp"
#include "ActionFactoryMock.hpp"
#include "ActionMock.hpp"

using namespace testing;

class ActionPerformerTest : public Server::AbstractTest
{
public:
    ActionPerformerTest() :
        action1(new Server::Game::Actions::ActionMock),
        action2(new Server::Game::Actions::ActionMock),
        ship1(new Common::Game::Object::ShipMock()),
        ship2(new Common::Game::Object::ShipMock())
    {
        ON_CALL(player, getFocusedObject()).WillByDefault(ReturnRef(*ship1));
        ON_CALL(player, getSelectedObject()).WillByDefault(ReturnRef(*ship2));
        ON_CALL(player, getId()).WillByDefault(Return(PLAYER_ID));

        ON_CALL(*ship1, getOwnerId()).WillByDefault(Return(PLAYER_ID));

        universe.add(ship1);
        universe.add(ship2);
    }

    ~ActionPerformerTest()
    {
        // hack for gmock bug: http://code.google.com/p/googlemock/issues/detail?id=79
        Mock::VerifyAndClear(&actionFactory);
    }

    Server::Network::ConnectionMock connection;
    Server::Game::PlayerContainerMock playerContainer;
    Common::Game::PlayerMock player;
    Server::Game::Actions::ActionFactoryMock actionFactory;
    boost::shared_ptr<Server::Game::Actions::IAction> action1;
    boost::shared_ptr<Server::Game::Actions::IAction> action2;
    Common::Game::Universe universe;
    boost::shared_ptr<Common::Game::Object::ShipMock> ship1;
    boost::shared_ptr<Common::Game::Object::ShipMock> ship2;

    static const int ATTACK_ID = 1;
    static const int ATTACK_PARAMETER = 2;
    static const int PLAYER_ID = 2;
};

TEST_F(ActionPerformerTest, Perform_DontFinish)
{
    boost::function<void()> globalCooldownTimerCallback;
    boost::function<void()> actionExecuteTimerCallback;

    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(1, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&globalCooldownTimerCallback));
    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(2, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&actionExecuteTimerCallback));
    EXPECT_CALL(actionFactory, create(_, _, ATTACK_ID, ATTACK_PARAMETER)).Times(1).WillRepeatedly(Return(action1));

    auto & actionMock = dynamic_cast<Server::Game::Actions::ActionMock&>(*action1);
    EXPECT_CALL(actionMock, start()).Times(1).WillRepeatedly(Return(Common::Game::TimeValue(2, 0)));
    EXPECT_CALL(actionMock, finish()).Times(0);

    Server::Game::Actions::ActionPerformer performer(actionFactory, universe, playerContainer);
    performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, false);
}

TEST_F(ActionPerformerTest, GlobalCooldown)
{
    boost::function<void()> globalCooldownTimerCallback;
    boost::function<void()> actionExecuteTimerCallback;

    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(1, 0), _)).Times(2).WillRepeatedly(SaveArg<1>(&globalCooldownTimerCallback));
    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(2, 0), _)).Times(2).WillRepeatedly(SaveArg<1>(&actionExecuteTimerCallback));
    EXPECT_CALL(playerContainer, getConnectionById(PLAYER_ID)).Times(1).WillRepeatedly(ReturnRef(connection));
    EXPECT_CALL(actionFactory, create(_, _, ATTACK_ID, ATTACK_PARAMETER)).Times(2).WillRepeatedly(Return(action1));

    auto & actionMock = dynamic_cast<Server::Game::Actions::ActionMock&>(*action1);
    ON_CALL(actionMock, start()).WillByDefault(Return(Common::Game::TimeValue(2, 0)));
    ON_CALL(actionMock, finish()).WillByDefault(Return(Common::Game::TimeValue()));

    Server::Game::Actions::ActionPerformer performer(actionFactory, universe, playerContainer);
    performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, false);

    // global cooldown timer is not expired yet
    EXPECT_ANY_THROW(performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, false));

    globalCooldownTimerCallback();

    // global cooldown has passed so we can do some action again without exception
    performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, false);
}

TEST_F(ActionPerformerTest, ActionExecutionFinishAfterTimerExpired)
{
    boost::function<void()> globalCooldownTimerCallback;
    boost::function<void()> actionExecuteTimerCallback;

    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(1, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&globalCooldownTimerCallback));
    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(2, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&actionExecuteTimerCallback));
    EXPECT_CALL(actionFactory, create(_, _, ATTACK_ID, ATTACK_PARAMETER)).Times(1).WillRepeatedly(Return(action1));
    EXPECT_CALL(playerContainer, getConnectionById(PLAYER_ID)).Times(1).WillOnce(ReturnRef(connection));
    EXPECT_CALL(connection, send(Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ActionStarted)))).Times(1);
    EXPECT_CALL(connection, send(Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ActionFinished)))).Times(1);

    auto & actionMock = dynamic_cast<Server::Game::Actions::ActionMock&>(*action1);
    EXPECT_CALL(actionMock, start()).Times(1).WillRepeatedly(Return(Common::Game::TimeValue(2, 0)));
    EXPECT_CALL(actionMock, finish()).Times(1).WillOnce(Return(Common::Game::TimeValue()));

    Server::Game::Actions::ActionPerformer performer(actionFactory, universe, playerContainer);
    performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, false);

    actionExecuteTimerCallback();
}

TEST_F(ActionPerformerTest, ActionExecutionFinishAfterTimerExpired_ActionTimeIsZero)
{
    boost::function<void()> globalCooldownTimerCallback;

    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(1, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&globalCooldownTimerCallback));
    EXPECT_CALL(actionFactory, create(_, _, ATTACK_ID, ATTACK_PARAMETER)).Times(1).WillRepeatedly(Return(action1));
    EXPECT_CALL(playerContainer, getConnectionById(PLAYER_ID)).Times(1).WillOnce(ReturnRef(connection));
    EXPECT_CALL(connection, send(Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ActionStarted)))).Times(1);
    EXPECT_CALL(connection, send(Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ActionFinished)))).Times(1);

    auto & actionMock = dynamic_cast<Server::Game::Actions::ActionMock&>(*action1);
    EXPECT_CALL(actionMock, start()).Times(1).WillRepeatedly(Return(Common::Game::TimeValue(0, 0)));
    EXPECT_CALL(actionMock, finish()).Times(1).WillOnce(Return(Common::Game::TimeValue()));

    Server::Game::Actions::ActionPerformer performer(actionFactory, universe, playerContainer);
    performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, false);
}

TEST_F(ActionPerformerTest, ActionExecutionNotFinishedUntilTimerExpires)
{
    boost::function<void()> globalCooldownTimerCallback;
    boost::function<void()> actionExecuteTimerCallback;

    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(1, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&globalCooldownTimerCallback));
    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(2, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&actionExecuteTimerCallback));
    EXPECT_CALL(actionFactory, create(_, _, ATTACK_ID, ATTACK_PARAMETER)).Times(1).WillRepeatedly(Return(action1));
    ON_CALL(playerContainer, getConnectionById(PLAYER_ID)).WillByDefault(ReturnRef(connection));

    auto & actionMock = dynamic_cast<Server::Game::Actions::ActionMock&>(*action1);
    EXPECT_CALL(actionMock, start()).Times(1).WillRepeatedly(Return(Common::Game::TimeValue(2, 0)));
    EXPECT_CALL(actionMock, finish()).Times(0);

    Server::Game::Actions::ActionPerformer performer(actionFactory, universe, playerContainer);
    performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, false);
}

#if 0
TEST_F(ActionPerformerTest, ActionExecuteWithLoop)
{
    boost::function<void()> globalCooldownTimerCallback;
    boost::function<void()> actionExecuteTimerCallback;

    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(1, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&globalCooldownTimerCallback));
    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(2, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&actionExecuteTimerCallback));
    EXPECT_CALL(actionFactory, create(_, _, ATTACK_ID, ATTACK_PARAMETER)).Times(2).WillRepeatedly(Return(action1));

    ON_CALL(playerContainer, getConnectionById(PLAYER_ID)).WillByDefault(ReturnRef(connection));

    auto & actionMock = dynamic_cast<Server::Game::Actions::ActionMock&>(*action1);
    EXPECT_CALL(actionMock, start()).Times(1).WillRepeatedly(Return(Common::Game::TimeValue(2, 0)));
    EXPECT_CALL(actionMock, finish()).Times(1).WillOnce(Return(Common::Game::TimeValue()));

    Server::Game::Actions::ActionPerformer performer(actionFactory, universe, playerContainer);
    performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, true);

    actionExecuteTimerCallback();
    globalCooldownTimerCallback();
}
#endif

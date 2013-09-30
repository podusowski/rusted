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
        ship2(new Common::Game::Object::ShipMock()),
        FOCUSED_OBJECT_ID(3),
        SELECTED_OBJECT_ID(4),
        ACTION_PARAMETERS(ATTACK_ID, ATTACK_PARAMETER, FOCUSED_OBJECT_ID, SELECTED_OBJECT_ID)
    {
        ON_CALL(player, getFocusedObject()).WillByDefault(ReturnRef(*ship1));
        ON_CALL(player, getSelectedObject()).WillByDefault(ReturnRef(*ship2));
        ON_CALL(player, getId()).WillByDefault(Return(PLAYER_ID));
        ON_CALL(playerContainer, getBy(_)).WillByDefault(ReturnRef(player));

        ON_CALL(*ship1, getOwnerId()).WillByDefault(Return(PLAYER_ID));
        ON_CALL(*ship1, getId()).WillByDefault(Return(FOCUSED_OBJECT_ID.get()));
        ON_CALL(*ship2, getId()).WillByDefault(Return(SELECTED_OBJECT_ID.get()));

        auto & action1Mock = dynamic_cast<Server::Game::Actions::ActionMock&>(*action1);
        auto & action2Mock = dynamic_cast<Server::Game::Actions::ActionMock&>(*action2);
        ON_CALL(action1Mock, isAbleToStart()).WillByDefault(Return(true));
        ON_CALL(action2Mock, isAbleToStart()).WillByDefault(Return(true));

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
    std::shared_ptr<Server::Game::Actions::IAction> action1;
    std::shared_ptr<Server::Game::Actions::IAction> action2;
    Common::Game::Universe universe;
    std::shared_ptr<Common::Game::Object::ShipMock> ship1;
    std::shared_ptr<Common::Game::Object::ShipMock> ship2;

    static const int ATTACK_ID = 1;
    static const int ATTACK_PARAMETER = 2;
    static const int PLAYER_ID = 2;
    const Common::Game::Object::ObjectBase::StrictId FOCUSED_OBJECT_ID;
    const Common::Game::Object::ObjectBase::Id SELECTED_OBJECT_ID;
    const Server::Game::Actions::ActionParameters ACTION_PARAMETERS;
};

TEST_F(ActionPerformerTest, Perform_DontFinish)
{
    boost::function<void()> globalCooldownTimerCallback;
    boost::function<void()> actionExecuteTimerCallback;

    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(1, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&globalCooldownTimerCallback));
    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(2, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&actionExecuteTimerCallback));
    EXPECT_CALL(actionFactory, create(_, _, ACTION_PARAMETERS)).Times(1).WillRepeatedly(Return(action1));

    auto & actionMock = dynamic_cast<Server::Game::Actions::ActionMock&>(*action1);
    EXPECT_CALL(actionMock, start()).Times(1).WillRepeatedly(Return(Common::Game::TimeValue(2, 0)));
    EXPECT_CALL(actionMock, finish()).Times(0);

    Server::Game::Actions::ActionPerformer performer(actionFactory, universe, playerContainer);
    performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, FOCUSED_OBJECT_ID, SELECTED_OBJECT_ID, false);
}

TEST_F(ActionPerformerTest, VariousActionCooldowns)
{
    boost::function<void()> globalCooldownTimerCallback;
    boost::function<void()> actionExecuteTimerCallback;
    boost::function<void()> actionCooldownTimerCallback;

    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(1, 0), _)).Times(2).WillRepeatedly(SaveArg<1>(&globalCooldownTimerCallback));
    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(2, 0), _)).Times(2).WillRepeatedly(SaveArg<1>(&actionExecuteTimerCallback));
    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(3, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&actionCooldownTimerCallback));

    // action finished, global cooldown expired
    EXPECT_CALL(playerContainer, getConnectionById(PLAYER_ID)).Times(3).WillRepeatedly(ReturnRef(connection));

    EXPECT_CALL(actionFactory, create(_, _, ACTION_PARAMETERS)).Times(2).WillRepeatedly(Return(action1));

    auto & actionMock = dynamic_cast<Server::Game::Actions::ActionMock&>(*action1);
    ON_CALL(actionMock, start()).WillByDefault(Return(Common::Game::TimeValue(2, 0)));
    ON_CALL(actionMock, finish()).WillByDefault(Return(Common::Game::TimeValue(3, 0)));

    Server::Game::Actions::ActionPerformer performer(actionFactory, universe, playerContainer);
    performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, FOCUSED_OBJECT_ID, SELECTED_OBJECT_ID, false);

    // global cooldown timer is not expired yet
    EXPECT_ANY_THROW(performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, FOCUSED_OBJECT_ID, SELECTED_OBJECT_ID, false));

    globalCooldownTimerCallback();

    // action is not finished yet, so we can't execute another
    EXPECT_ANY_THROW(performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, FOCUSED_OBJECT_ID, SELECTED_OBJECT_ID, false));

    actionExecuteTimerCallback();

    // after action cooldown isn't expired yet so we can't execute the same action
    EXPECT_ANY_THROW(performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, FOCUSED_OBJECT_ID, SELECTED_OBJECT_ID, false));

    actionCooldownTimerCallback();

    // action is finished and its cooldown expired so we can perform it again
    performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, FOCUSED_OBJECT_ID, SELECTED_OBJECT_ID, false);
}

TEST_F(ActionPerformerTest, ActionExecutionFinishAfterTimerExpired)
{
    boost::function<void()> globalCooldownTimerCallback;
    boost::function<void()> actionExecuteTimerCallback;

    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(1, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&globalCooldownTimerCallback));
    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(2, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&actionExecuteTimerCallback));
    EXPECT_CALL(actionFactory, create(_, _, ACTION_PARAMETERS)).Times(1).WillRepeatedly(Return(action1));
    EXPECT_CALL(playerContainer, getConnectionById(PLAYER_ID)).Times(2).WillRepeatedly(ReturnRef(connection));

    // network
    EXPECT_CALL(connection, send(Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::GlobalCooldownActivated)))).Times(1);
    EXPECT_CALL(connection, send(Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ActionStarted)))).Times(1);
    EXPECT_CALL(connection, send(Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ActionFinished)))).Times(1);
    EXPECT_CALL(connection, send(Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ActionCooldownExpired)))).Times(1);

    auto & actionMock = dynamic_cast<Server::Game::Actions::ActionMock&>(*action1);
    EXPECT_CALL(actionMock, start()).Times(1).WillRepeatedly(Return(Common::Game::TimeValue(2, 0)));
    EXPECT_CALL(actionMock, finish()).Times(1).WillOnce(Return(Common::Game::TimeValue()));

    Server::Game::Actions::ActionPerformer performer(actionFactory, universe, playerContainer);
    performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, FOCUSED_OBJECT_ID, SELECTED_OBJECT_ID, false);

    actionExecuteTimerCallback();
}

TEST_F(ActionPerformerTest, ActionExecutionFinishAfterTimerExpired_ActionTimeIsZero)
{
    boost::function<void()> globalCooldownTimerCallback;

    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(1, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&globalCooldownTimerCallback));
    EXPECT_CALL(actionFactory, create(_, _, ACTION_PARAMETERS)).Times(1).WillRepeatedly(Return(action1));
    EXPECT_CALL(playerContainer, getConnectionById(PLAYER_ID)).Times(2).WillRepeatedly(ReturnRef(connection));

    // network
    EXPECT_CALL(connection, send(Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::GlobalCooldownActivated)))).Times(1);
    EXPECT_CALL(connection, send(Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ActionStarted)))).Times(1);
    EXPECT_CALL(connection, send(Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ActionFinished)))).Times(1);
    EXPECT_CALL(connection, send(Property(&Common::Messages::AbstractMessage::getId, Eq(Common::Messages::Id::ActionCooldownExpired)))).Times(1);

    auto & actionMock = dynamic_cast<Server::Game::Actions::ActionMock&>(*action1);
    EXPECT_CALL(actionMock, start()).Times(1).WillRepeatedly(Return(Common::Game::TimeValue(0, 0)));
    EXPECT_CALL(actionMock, finish()).Times(1).WillOnce(Return(Common::Game::TimeValue()));

    Server::Game::Actions::ActionPerformer performer(actionFactory, universe, playerContainer);
    performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, FOCUSED_OBJECT_ID, SELECTED_OBJECT_ID, false);
}

TEST_F(ActionPerformerTest, ActionExecutionNotFinishedUntilTimerExpires)
{
    boost::function<void()> globalCooldownTimerCallback;
    boost::function<void()> actionExecuteTimerCallback;

    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(1, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&globalCooldownTimerCallback));
    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(2, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&actionExecuteTimerCallback));
    EXPECT_CALL(actionFactory, create(_, _, ACTION_PARAMETERS)).Times(1).WillRepeatedly(Return(action1));
    ON_CALL(playerContainer, getConnectionById(PLAYER_ID)).WillByDefault(ReturnRef(connection));

    auto & actionMock = dynamic_cast<Server::Game::Actions::ActionMock&>(*action1);
    EXPECT_CALL(actionMock, start()).Times(1).WillRepeatedly(Return(Common::Game::TimeValue(2, 0)));
    EXPECT_CALL(actionMock, finish()).Times(0);

    Server::Game::Actions::ActionPerformer performer(actionFactory, universe, playerContainer);
    performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, FOCUSED_OBJECT_ID, SELECTED_OBJECT_ID, false);
}

TEST_F(ActionPerformerTest, ActionExecuteWithLoop)
{
    boost::function<void()> globalCooldownTimerCallback;
    boost::function<void()> actionExecuteTimerCallback;
    boost::function<void()> actionCooldownTimerCallback;

    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(1, 0), _)).Times(2).WillRepeatedly(SaveArg<1>(&globalCooldownTimerCallback));
    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(2, 0), _)).Times(2).WillRepeatedly(SaveArg<1>(&actionExecuteTimerCallback));
    EXPECT_CALL(getTimeMock(), createTimer(Common::Game::TimeValue(3, 0), _)).Times(1).WillRepeatedly(SaveArg<1>(&actionCooldownTimerCallback));

    EXPECT_CALL(actionFactory, create(_, _, ACTION_PARAMETERS)).Times(2).WillRepeatedly(Return(action1));

    ON_CALL(playerContainer, getConnectionById(PLAYER_ID)).WillByDefault(ReturnRef(connection));

    auto & actionMock = dynamic_cast<Server::Game::Actions::ActionMock&>(*action1);
    EXPECT_CALL(actionMock, start()).Times(2).WillRepeatedly(Return(Common::Game::TimeValue(2, 0)));
    EXPECT_CALL(actionMock, finish()).Times(1).WillOnce(Return(Common::Game::TimeValue(3, 0)));

    Server::Game::Actions::ActionPerformer performer(actionFactory, universe, playerContainer);
    performer.perform(connection, player, ATTACK_ID, ATTACK_PARAMETER, FOCUSED_OBJECT_ID, SELECTED_OBJECT_ID, true);

    actionExecuteTimerCallback();
    globalCooldownTimerCallback();
    actionCooldownTimerCallback();
}

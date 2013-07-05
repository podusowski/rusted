#include <gtest/gtest.h>

#include "Core/Component.hpp"
#include "Core/Connection.hpp"
#include "Preconditions.hpp"
#include "UserFunctions.hpp"

using namespace Common::Messages;

/* Actions are all things you can do with focused object (the one you're controlling) on selected object.
 * For example, attack other ship is an action. Client should fetch the actions and display they as
 * action buttons.
 *
 * To perform an action, client have to select some object first (it's not nessesary to do this just
 * before performing the action - it can be done when player select object by mouse).
 */

TEST(ActionsSct, FetchAvailableActions)
{
    SCT::Component component("SampleDataBase.xml");
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 

    // actions are attached to focused object so we have to have one
    Common::Messages::FocusObject focusObject;
    focusObject.id = 1;
    connection1->send(focusObject);

    // client should fetch available action list explicitly
    Common::Messages::FetchAvailableActions fetchAvailableActions;
    fetchAvailableActions.shipId = 1;
    connection1->send(fetchAvailableActions);

    auto availableActions = connection1->receive<Common::Messages::AvailableActions>();
    ASSERT_EQ(5, availableActions->actions.size());

    // attack
    EXPECT_EQ(1, availableActions->actions[0].id); // action id
    EXPECT_EQ(1, availableActions->actions[0].parameter); // action parameter

    // attack
    EXPECT_EQ(1, availableActions->actions[1].id); // action id
    EXPECT_EQ(2, availableActions->actions[1].parameter); // action parameter

    // build 
    EXPECT_EQ(2, availableActions->actions[2].id);
    EXPECT_EQ(1, availableActions->actions[2].parameter);

    // gather
    EXPECT_EQ(3, availableActions->actions[3].id);
    EXPECT_EQ(1, availableActions->actions[3].parameter);

    // transfer
    EXPECT_EQ(4, availableActions->actions[4].id);
    EXPECT_EQ(1, availableActions->actions[4].parameter);
}

TEST(ActionsSct, AttackObject)
{
    SCT::Component component("SampleDataBase.xml");
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 
    boost::shared_ptr<SCT::Connection> connection2 = authorizeUser(component, "user2", "password"); 

    // client has to focus its own object 
    Common::Messages::FocusObject focusObject;
    focusObject.id = 1;
    connection1->send(focusObject);

    // client has to select some object
    Common::Messages::SelectObject selectObject;
    selectObject.id = 5;
    connection1->send(selectObject);

    // execute action 1 - attack 
    Common::Messages::ExecuteAction executeAction;
    executeAction.id = 1;
    executeAction.loop = false;
    connection1->send(executeAction);

    connection1->receive<Common::Messages::GlobalCooldownActivated>();
    connection1->receive<Common::Messages::ActionStarted>();

    // clients should receive emit effect requests
    auto emitMovingMeshEffect = connection1->receive<Common::Messages::EmitMovingMeshEffect>();
    EXPECT_EQ(1, emitMovingMeshEffect->fromX);
    EXPECT_EQ(1, emitMovingMeshEffect->fromY);
    EXPECT_EQ(1, emitMovingMeshEffect->fromZ);

    EXPECT_EQ(10000, emitMovingMeshEffect->toX);
    EXPECT_EQ(1, emitMovingMeshEffect->toY);
    EXPECT_EQ(1, emitMovingMeshEffect->toZ);

    EXPECT_EQ(1000, emitMovingMeshEffect->speed);

    // some time will pass so global cooldown will expire at this point
    connection1->interleave(
        [] (Common::Messages::GlobalCooldownExpired &) -> void {},
        [] (Common::Messages::EmitExplosionEffect &) -> void {}
    );

    // we should also receive shipinfo with condition after the attack
    auto objectIntegrity = connection1->receive<Common::Messages::ObjectIntegrity>();
    ASSERT_EQ(5, objectIntegrity->id);
    ASSERT_EQ(90, objectIntegrity->integrity);

    connection1->receive<Common::Messages::ActionFinished>();
    connection1->receive<Common::Messages::ActionCooldownExpired>();

    // other player should also get this stuff
    connection2->receive<Common::Messages::EmitMovingMeshEffect>();
    connection2->receive<Common::Messages::EmitExplosionEffect>();
    connection2->receive<Common::Messages::ObjectIntegrity>();
}

TEST(ActionsSct, BuildShip)
{
    SCT::Component component("SampleDataBase.xml");
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 
    boost::shared_ptr<SCT::Connection> connection2 = authorizeUser(component, "user2", "password"); 

    // client has to focus its own object 
    Common::Messages::FocusObject focusObject;
    focusObject.id = 1;
    connection1->send(focusObject);

    // execute action 2 - buildship
    Common::Messages::ExecuteAction executeAction;
    executeAction.id = 2;
    executeAction.parameter = 2;
    executeAction.loop = false;
    connection1->send(executeAction);

    connection1->receive<Common::Messages::GlobalCooldownActivated>();
    connection1->receive<Common::Messages::ActionStarted>();

    // receive cargo info of focused object since some resources
    // were used to build new ship
    auto objectCargoInfo = connection1->receive<Common::Messages::ObjectCargoInfo>();
    EXPECT_EQ(0, objectCargoInfo->carbon);
    EXPECT_EQ(5, objectCargoInfo->helium);

    connection1->receive<Common::Messages::GlobalCooldownExpired>();

    // receive info about newly created ship
    auto shipInfo = connection1->receive<Common::Messages::ShipInfo>();
    EXPECT_EQ(20, shipInfo->speed);
    EXPECT_EQ(200, shipInfo->integrity);

    connection1->receive<Common::Messages::ShipCourseInfo>();
    connection1->receive<Common::Messages::ObjectCargoInfo>();
    connection1->receive<Common::Messages::ActionFinished>();
    connection1->receive<Common::Messages::ActionCooldownExpired>();

    connection2->receive<Common::Messages::ShipInfo>();
    connection2->receive<Common::Messages::ShipCourseInfo>();
    connection2->receive<Common::Messages::ObjectCargoInfo>();
}

TEST(ActionsSct, Gather)
{
    SCT::Component component("SampleDataBase.xml");
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 

    // client has to focus its own gather capable object 
    Common::Messages::FocusObject focusObject;
    focusObject.id = 1;
    connection1->send(focusObject);

    // select asteroid
    Common::Messages::SelectObject selectObject;
    selectObject.id = 3;
    connection1->send(selectObject);

    // execute action 3 - gather
    Common::Messages::ExecuteAction executeAction;
    executeAction.id = 3;
    //executeAction.parameter = 2;
    executeAction.loop = false;
    connection1->send(executeAction);

    connection1->receive<Common::Messages::GlobalCooldownActivated>();
    connection1->receive<Common::Messages::ActionStarted>();

    connection1->receive<Common::Messages::GlobalCooldownExpired>();

    // receive info about player ship cargohold
    auto shipCargoInfo = connection1->receive<Common::Messages::ObjectCargoInfo>();
    EXPECT_EQ(1, shipCargoInfo->id);
    EXPECT_EQ(20, shipCargoInfo->carbon);
    EXPECT_EQ(20, shipCargoInfo->helium);

    // receive info about asteroid cargohold
    auto asteroidCargoInfo = connection1->receive<Common::Messages::ObjectCargoInfo>();
    EXPECT_EQ(3, asteroidCargoInfo->id);
    EXPECT_EQ(90, asteroidCargoInfo->carbon);
    EXPECT_EQ(90, asteroidCargoInfo->helium);

    connection1->receive<Common::Messages::ActionFinished>();
    connection1->receive<Common::Messages::ActionCooldownExpired>();
}

TEST(ActionsSct, Transfer)
{
    SCT::Component component("SampleDataBase.xml");
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 
    boost::shared_ptr<SCT::Connection> connection2 = authorizeUser(component, "user2", "password"); 

    // focus some ship
    Common::Messages::FocusObject focusObject;
    focusObject.id = 1;
    connection1->send(focusObject);

    // select some user2's ship
    Common::Messages::SelectObject selectObject;
    selectObject.id = 2;
    connection1->send(selectObject);

    // execute action 4 - transfer
    Common::Messages::ExecuteAction executeAction;
    executeAction.id = 4;
    executeAction.loop = false;
    connection1->send(executeAction);

    connection1->receive<Common::Messages::GlobalCooldownActivated>();
    connection1->receive<Common::Messages::ActionStarted>();

    // cargo info
    connection1->receive<Common::Messages::ObjectCargoInfo>();
    connection2->receive<Common::Messages::ObjectCargoInfo>();

    connection1->receive<Common::Messages::ActionFinished>();

    connection1->interleave(
        [](Common::Messages::GlobalCooldownExpired &) -> void {},
        [](Common::Messages::ActionCooldownExpired &) -> void {}
    );
}

TEST(ActionsSct, ExecuteActionInLoop)
{
    SCT::Component component("SampleDataBase.xml");
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 

    // client has to focus its own gather capable object 
    Common::Messages::FocusObject focusObject;
    focusObject.id = 1;
    connection1->send(focusObject);

    // select asteroid
    Common::Messages::SelectObject selectObject;
    selectObject.id = 3;
    connection1->send(selectObject);

    // execute action 3 - gather
    Common::Messages::ExecuteAction executeAction;
    executeAction.id = 3;
    //executeAction.parameter = 2;
    executeAction.loop = true;
    connection1->send(executeAction);

    for (int i = 0; i < 2; i++)
    {
        connection1->receive<Common::Messages::GlobalCooldownActivated>();
        connection1->receive<Common::Messages::ActionStarted>();

        connection1->receive<Common::Messages::GlobalCooldownExpired>();

        // receive info about player ship cargohold
        connection1->receive<Common::Messages::ObjectCargoInfo>();

        // receive info about asteroid cargohold
        connection1->receive<Common::Messages::ObjectCargoInfo>();

        connection1->receive<Common::Messages::ActionFinished>();
        connection1->receive<Common::Messages::ActionCooldownExpired>();
    }
}


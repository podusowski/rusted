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
    ASSERT_EQ(3, availableActions->actions.size());

    EXPECT_EQ(1, availableActions->actions[0].get<0>()); // action id
    EXPECT_EQ(1, availableActions->actions[0].get<1>()); // action parameter
    EXPECT_EQ("attack", availableActions->actions[0].get<2>());

    EXPECT_EQ(1, availableActions->actions[1].get<0>()); // action id
    EXPECT_EQ(2, availableActions->actions[1].get<1>()); // action parameter
    EXPECT_EQ("attack", availableActions->actions[1].get<2>());

    EXPECT_EQ(2, availableActions->actions[2].get<0>());
    EXPECT_EQ(1, availableActions->actions[2].get<1>());
    EXPECT_EQ("build", availableActions->actions[2].get<2>());
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

    // execute hardcoded action 1 - attack 
    Common::Messages::ExecuteAction executeAction;
    executeAction.id = 1;
    connection1->send(executeAction);

    // clients should receive emit effect requests
    auto emitMovingMeshEffect = connection1->receive<Common::Messages::EmitMovingMeshEffect>();
    EXPECT_EQ(1, emitMovingMeshEffect->fromX);
    EXPECT_EQ(1, emitMovingMeshEffect->fromY);
    EXPECT_EQ(1, emitMovingMeshEffect->fromZ);

    EXPECT_EQ(10000, emitMovingMeshEffect->toX);
    EXPECT_EQ(1, emitMovingMeshEffect->toY);
    EXPECT_EQ(1, emitMovingMeshEffect->toZ);

    EXPECT_EQ(1000, emitMovingMeshEffect->speed);

    // user1 should also receive AttackObject to know what he has done
    auto attackObject1 = connection1->receive<Common::Messages::AttackObject>();
    ASSERT_EQ(1, attackObject1->attackerId);
    ASSERT_EQ(5, attackObject1->attackedId);

    // we should also receive shipinfo with condition after the attack
    auto shipInfo1 = connection1->receive<Common::Messages::ShipInfo>();
    ASSERT_EQ(5, shipInfo1->id);
    ASSERT_EQ(90, shipInfo1->integrity);

    // other player should also get this stuff
    connection2->receive<Common::Messages::EmitMovingMeshEffect>();
    connection2->receive<Common::Messages::AttackObject>();
    connection2->receive<Common::Messages::ShipInfo>();
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
    connection1->send(executeAction);

    connection1->receive<Common::Messages::ActionStarted>();
    connection1->receive<Common::Messages::GlobalCooldownExpired>();
    connection1->receive<Common::Messages::ActionFinished>();

    // receive info about newly created ship
    auto shipInfo = connection1->receive<Common::Messages::ShipInfo>();
    EXPECT_EQ(20, shipInfo->speed);
    EXPECT_EQ(200, shipInfo->integrity);

    connection1->receive<Common::Messages::ShipCourseInfo>();

    connection2->receive<Common::Messages::ShipInfo>();
    connection2->receive<Common::Messages::ShipCourseInfo>();
}


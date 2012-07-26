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

    // client should fetch available action list explicitly
    Common::Messages::FetchAvailableActions fetchAvailableActions;
    connection1->send(fetchAvailableActions);

    // currently there is one hardcoded action
    auto availableActions = connection1->receive<Common::Messages::AvailableActions>();
    EXPECT_EQ(1, availableActions->actions.size());
    EXPECT_EQ(1, availableActions->actions[0].get<0>());
    EXPECT_EQ("attack", availableActions->actions[0].get<1>());
}

TEST(ActionsSct, AttackObject)
{
    SCT::Component component("SampleDataBase.xml");
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 

    // client has to focus its own object 
    Common::Messages::FocusObject focusObject;
    focusObject.id = 1;
    connection1->send(focusObject);

    // client has to select some object
    Common::Messages::SelectObject selectObject;
    selectObject.id = 2;
    connection1->send(selectObject);

    // execute hardcoded action 1 - attack 
    Common::Messages::ExecuteAction executeAction;
    executeAction.id = 1;
    connection1->send(executeAction);

    // user1 should also receive AttackObject to know what he was done
    connection1->receive<Common::Messages::AttackObject>();

    // we should also receive shipinfo with condition after the attack
    auto shipInfo1 = connection1->receive<Common::Messages::ShipInfo>();
    ASSERT_EQ(2, shipInfo1->id);
    ASSERT_EQ(90, shipInfo1->integrity);
}


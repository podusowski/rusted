#include <gtest/gtest.h>

#include "Core/Component.hpp"
#include "Core/Connection.hpp"
#include "Preconditions.hpp"
#include "UserFunctions.hpp"

using namespace Common::Messages;

/* Actions are all things you can do with selected object (the one you're controlling) on focused object.
 * For example, attack other ship is a action. Client should fetch the actions and display they as
 * action buttons.
 *
 * To perform an action, client have to focus some object first (it's not nessesary to do this just
 * before performing the action - it can be done when player focus object by mouse).
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

    // client have to focus the object first
    Common::Messages::FocusObject focusObject;
    focusObject.id = 2;
    connection1->send(focusObject);

    // TODO
}

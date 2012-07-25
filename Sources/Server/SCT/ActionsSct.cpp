#include <gtest/gtest.h>

#include "Core/Component.hpp"
#include "Core/Connection.hpp"
#include "Preconditions.hpp"
#include "UserFunctions.hpp"

using namespace Common::Messages;

/* currently there is one hardcoded action - attack */
TEST(ActionsSct, FetchAvailableActions)
{
	SCT::Component component("SampleDataBase.xml");
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 

    Common::Messages::FetchAvailableActions fetchAvailableActions;
    connection1->send(fetchAvailableActions);

    auto availableActions = connection1->receive<Common::Messages::AvailableActions>();
    EXPECT_EQ(1, availableActions->actions.size());
    EXPECT_EQ(1, availableActions->actions[0].get<0>());
    EXPECT_EQ("attack", availableActions->actions[0].get<1>());
}

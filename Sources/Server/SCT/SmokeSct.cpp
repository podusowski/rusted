#include <gtest/gtest.h>

#include "Core/Component.hpp"
#include "Core/Connection.hpp"
#include "Preconditions.hpp"
#include "UserFunctions.hpp"

using namespace Common::Messages;

TEST(SmokeSct, Smoke)
{
	SCT::Component component("SampleDataBase.xml");
    component.start();

    for (int i = 0; i < 100; i++)
    {
        boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 

        Common::Messages::FetchAvailableActions fetchAvailableActions;
        fetchAvailableActions.shipId = 1;
        connection1->send(fetchAvailableActions);
        auto availableActions = connection1->receive<Common::Messages::AvailableActions>();

        Common::Messages::FocusObject focusObject;
        focusObject.id = 1;
        connection1->send(focusObject);

        Common::Messages::SelectObject selectObject;
        selectObject.id = 2;
        connection1->send(selectObject);
    }
}


#include <gtest/gtest.h>

#include "Core/Component.hpp"
#include "Core/Connection.hpp"
#include "Preconditions.hpp"
#include "UserFunctions.hpp"

using namespace Common::Messages;

TEST(ObjectsSct, FetchVisibleObjects)
{
	SCT::Component component("SampleDataBase.xml");
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 

    Common::Messages::GetVisibleObjects getVisibleObjects;
    connection1->send(getVisibleObjects);

    boost::shared_ptr<Common::Messages::VisibleObjects> visibleObjects = connection1->receive<Common::Messages::VisibleObjects>();
    EXPECT_EQ(4, visibleObjects->objects.size());
}

TEST(ObjectsSct, GetObjectInfo_Ship)
{
	SCT::Component component("SampleDataBase.xml");
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 

    Common::Messages::GetObjectInfo getObjectInfo;
    getObjectInfo.id = 1;
    connection1->send(getObjectInfo);

    connection1->receive<Common::Messages::ShipInfo>();
}

TEST(ObjectsSct, GetObjectInfo_StaticObject)
{
	SCT::Component component("SampleDataBase.xml");
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 

    Common::Messages::GetObjectInfo getObjectInfo;
    getObjectInfo.id = 3;
    connection1->send(getObjectInfo);

    connection1->receive<Common::Messages::StaticObjectInfoResp>();
}

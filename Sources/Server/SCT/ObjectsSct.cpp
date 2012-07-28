#include <gtest/gtest.h>

#include "Core/Component.hpp"
#include "Core/Connection.hpp"
#include "Preconditions.hpp"
#include "UserFunctions.hpp"

using namespace Common::Messages;

/* This flows represents the way client is aware of various object present in the game */

TEST(ObjectsSct, FetchVisibleObjects)
{
	SCT::Component component("SampleDataBase.xml");
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 

    Common::Messages::GetVisibleObjects getVisibleObjects;
    connection1->send(getVisibleObjects);

    // VisibleObjects contains only ids of the objects that are visible by the client, to get
    // more information, GetObjectInfo should be used for each of them

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

    auto shipInfo = connection1->receive<Common::Messages::ShipInfo>();
    EXPECT_EQ(100, shipInfo->integrity);
}

TEST(ObjectsSct, GetObjectInfo_StaticObject)
{
	SCT::Component component("SampleDataBase.xml");
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 

    Common::Messages::GetObjectInfo getObjectInfo;
    getObjectInfo.id = 3;
    connection1->send(getObjectInfo);

    auto staticObjectInfoResp = connection1->receive<Common::Messages::StaticObjectInfoResp>();
    EXPECT_EQ(getObjectInfo.id, staticObjectInfoResp->staticObjectId);
    EXPECT_EQ(100, staticObjectInfoResp->x);
    EXPECT_EQ(100, staticObjectInfoResp->y);
    EXPECT_EQ(100, staticObjectInfoResp->z);
}

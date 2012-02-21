#include <gtest/gtest.h>

#include "Core/Component.hpp"
#include "Core/Connection.hpp"
#include "Preconditions.hpp"
#include "UserFunctions.hpp"

using namespace Common::Messages;

TEST(ObjectsSct, FetchVisibleObjects)
{
    std::string dbFile = "SampleDataBase.xml";

	SCT::Component component;
    component.setConfigValue("--database.provider", "xml");
    component.setConfigValue("--database.xml.filename", dbFile);
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 

    Common::Messages::GetVisibleObjects getVisibleObjects;
    connection1->send(getVisibleObjects);

    boost::shared_ptr<Common::Messages::AbstractMessage> visibleObjects = connection1->receive();
    EXPECT_EQ(4, dynamic_cast<Common::Messages::VisibleObjects&>(*visibleObjects).objects.size());
}

TEST(ObjectsSct, GetObjectInfo_Ship)
{
    std::string dbFile = "SampleDataBase.xml";

	SCT::Component component;
    component.setConfigValue("--database.provider", "xml");
    component.setConfigValue("--database.xml.filename", dbFile);
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 

    Common::Messages::GetObjectInfo getObjectInfo;
    getObjectInfo.id = 1;
    connection1->send(getObjectInfo);

    boost::shared_ptr<Common::Messages::AbstractMessage> objectInfo = connection1->receive();
    EXPECT_EQ(Common::Messages::Id::ShipInfo, objectInfo->getId());
}

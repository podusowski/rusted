#include <gtest/gtest.h>

#include "Cake/Threading/Thread.hpp"

#include "Core/Component.hpp"
#include "Core/Connection.hpp"
#include "Preconditions.hpp"
#include "UserFunctions.hpp"

using namespace Common::Messages;

class ShipSct : public testing::Test 
{
public:
    boost::shared_ptr<Common::Messages::ShipInfo> procedureEntityGetInfo(SCT::Connection & connection, int entityId);
    void procedureEntityChangeCourse(SCT::Connection & connection, int entityId, int x, int y, int z);
};

TEST_F(ShipSct, ChangeShipCourse)
{
	SCT::Component component("SampleDataBase.xml");
    component.start();

    auto connection = authorizeUser(component, "user1", "password");

    // make sure the ships is where we want it to be
    Common::Messages::GetObjectInfo getObjectInfo;
    getObjectInfo.id = 1;

    connection->send(getObjectInfo);

    auto shipInfo = connection->receive<Common::Messages::ShipInfo>();
    EXPECT_EQ(1, shipInfo->player_id);
    EXPECT_EQ(1, shipInfo->x);
    EXPECT_EQ(1, shipInfo->y);
    EXPECT_EQ(1, shipInfo->z);

    // change course
    Common::Messages::EntityChangeCourseReq entityChangeCourseReq;
    entityChangeCourseReq.entityId = 1;
    entityChangeCourseReq.courseX = 2;
    entityChangeCourseReq.courseY = 1;
    entityChangeCourseReq.courseZ = 1;
    connection->send(entityChangeCourseReq);

    Cake::Threading::Thread::wait(2.0);

    connection->send(getObjectInfo);
    auto shipInfo2 = connection->receive<Common::Messages::ShipInfo>();
    EXPECT_TRUE(1 == shipInfo2->player_id);
    EXPECT_TRUE(2 == shipInfo2->x);
    EXPECT_TRUE(1 == shipInfo2->y);
    EXPECT_TRUE(1 == shipInfo2->z);
}

TEST_F(ShipSct, ChangeShipCourseAnotherPlayerIsNotified)
{
    std::string dbFile = "SampleDataBase.xml";

	SCT::Component component;
    component.setConfigValue("--database.provider", "xml");
    component.setConfigValue("--database.xml.filename", dbFile);
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 
    boost::shared_ptr<SCT::Connection> connection2 = authorizeUser(component, "user2", "password"); 

    // we will need this later
    Common::Messages::RustedTimeEpochReq rustedTimeEpochReq;
    connection1->send(rustedTimeEpochReq);
    auto rustedTimeEpochResp = connection1->receive<Common::Messages::RustedTimeEpochResp>();

    procedureEntityChangeCourse(*connection1, 1, 2, 1, 1);

    // second player gets notified
    auto shipCourseInfo = connection2->receive<Common::Messages::ShipCourseInfo>();
    EXPECT_EQ(1, shipCourseInfo->objectId);
    EXPECT_EQ(1, shipCourseInfo->positionX);
    EXPECT_EQ(1, shipCourseInfo->positionY);
    EXPECT_EQ(1, shipCourseInfo->positionZ);
    EXPECT_EQ(2, shipCourseInfo->destinationX);
    EXPECT_EQ(1, shipCourseInfo->destinationY);
    EXPECT_EQ(1, shipCourseInfo->destinationZ);

    // start time should be more or less (1 seconds) equal to epoch received earlier
    EXPECT_TRUE(
        rustedTimeEpochResp->time <= shipCourseInfo->startTimeSeconds && 
        rustedTimeEpochResp->time + 1 >= shipCourseInfo->startTimeSeconds
    );
}

TEST_F(ShipSct, ChangeShipCourseAnotherPlayerWasConnectedEarlier)
{
    std::string dbFile = "SampleDataBase.xml";

	SCT::Component component;
    component.setConfigValue("--database.provider", "xml");
    component.setConfigValue("--database.xml.filename", dbFile);
    component.start();

    {
        auto connection2 = authorizeUser(component, "user1", "password"); 
        procedureEntityChangeCourse(*connection2, 1, 10, 1, 1);
    }

    // flush previous connection
    Cake::Threading::Thread::wait(1);

    auto connection = authorizeUser(component, "user1", "password"); 

    procedureEntityChangeCourse(*connection, 1, 2, 1, 1);

    Cake::Threading::Thread::wait(0, 500);
}

boost::shared_ptr<Common::Messages::ShipInfo> ShipSct::procedureEntityGetInfo(
    SCT::Connection & connection, 
    int entityId)
{
    Common::Messages::EntityGetInfoReq entityGetInfoReq;
    entityGetInfoReq.id = entityId;

    connection.send(entityGetInfoReq);

    auto shipInfo = connection.receive<Common::Messages::ShipInfo>();

    return shipInfo;
}

void ShipSct::procedureEntityChangeCourse(
    SCT::Connection & connection, 
    int entityId, 
    int x, int y, int z)
{
    Common::Messages::EntityChangeCourseReq entityChangeCourseReq;
    entityChangeCourseReq.entityId = entityId;
    entityChangeCourseReq.courseX = x;
    entityChangeCourseReq.courseY = y;
    entityChangeCourseReq.courseZ = z;
    connection.send(entityChangeCourseReq); 
}


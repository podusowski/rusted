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
    void procedureEntityChangeCourse(SCT::Connection & connection, int entityId, int x, int y, int z);
};

TEST_F(ShipSct, ChangeShipCourse)
{
    SCT::Component component;
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
    connection->receive<Common::Messages::ShipCourseInfo>();
    connection->receive<Common::Messages::ObjectCargoInfo>();

    // change course
    Common::Messages::ChangeShipCourse entityChangeCourseReq;
    entityChangeCourseReq.shipId = 1;
    entityChangeCourseReq.x = 2;
    entityChangeCourseReq.y = 1;
    entityChangeCourseReq.z = 1;
    connection->send(entityChangeCourseReq);

    connection->receive<Common::Messages::ShipCourseInfo>();

    Cake::Threading::Thread::wait(2.0);

    connection->send(getObjectInfo);
    auto shipInfo2 = connection->receive<Common::Messages::ShipInfo>();
    //TODO
    /*
    EXPECT_EQ(1, shipInfo2->player_id);
    EXPECT_EQ(2, shipInfo2->x);
    EXPECT_EQ(1, shipInfo2->y);
    EXPECT_EQ(1, shipInfo2->z);
    */
}

TEST_F(ShipSct, ChangeShipCourseAnotherPlayerIsNotified)
{
    SCT::Component component;
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
    // TODO: match controlPoints

    // start time should be more or less (1 seconds) equal to epoch received earlier
    EXPECT_TRUE(
        rustedTimeEpochResp->seconds <= shipCourseInfo->startTimeSeconds && 
        rustedTimeEpochResp->seconds + 1 >= shipCourseInfo->startTimeSeconds
    );
}

TEST_F(ShipSct, ChangeShipCourseAnotherPlayerWasConnectedEarlier)
{
    SCT::Component component;
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

void ShipSct::procedureEntityChangeCourse(
    SCT::Connection & connection, 
    int entityId, 
    int x, int y, int z)
{
    Common::Messages::ChangeShipCourse entityChangeCourseReq;
    entityChangeCourseReq.shipId = entityId;
    entityChangeCourseReq.x = x;
    entityChangeCourseReq.y = y;
    entityChangeCourseReq.z = z;
    connection.send(entityChangeCourseReq); 
}


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
    std::auto_ptr<Common::Messages::AbstractMessage> procedureEntityGetInfo(SCT::Connection & connection, int entityId);
    void procedureEntityChangeCourse(SCT::Connection & connection, int entityId, int x, int y, int z);
};

TEST_F(ShipSct, testEntityChangeCourseReq)
{
    SCT::PreconditionPlayerLoggedIn precondition;
    SCT::Connection & connection = precondition.getConnection();

    std::auto_ptr<Common::Messages::AbstractMessage> entityGetInfoRespA = procedureEntityGetInfo(connection, 1);
    Common::Messages::ShipInfo & entitiesGetInfoResp = dynamic_cast<Common::Messages::ShipInfo&>(*entityGetInfoRespA);
    EXPECT_TRUE(1 == entitiesGetInfoResp.player_id);
    EXPECT_TRUE(1 == entitiesGetInfoResp.x);
    EXPECT_TRUE(1 == entitiesGetInfoResp.y);
    EXPECT_TRUE(1 == entitiesGetInfoResp.z);

    procedureEntityChangeCourse(connection, 1, 2, 1, 1);

    Cake::Threading::Thread::wait(2.0);

    std::auto_ptr<Common::Messages::AbstractMessage> entityGetInfoRespA2 = procedureEntityGetInfo(connection, 1);
    Common::Messages::ShipInfo & entitiesGetInfoResp2 = dynamic_cast<Common::Messages::ShipInfo&>(*entityGetInfoRespA2);
    EXPECT_TRUE(1 == entitiesGetInfoResp2.player_id);
    EXPECT_TRUE(2 == entitiesGetInfoResp2.x);
    EXPECT_TRUE(1 == entitiesGetInfoResp2.y);
    EXPECT_TRUE(1 == entitiesGetInfoResp2.z);
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

    procedureEntityChangeCourse(*connection1, 1, 2, 1, 1);

    // second player gets notified
    std::auto_ptr<AbstractMessage> entityChangeCourse2 = connection2->receive();
}

std::auto_ptr<Common::Messages::AbstractMessage> ShipSct::procedureEntityGetInfo(
    SCT::Connection & connection, 
    int entityId)
{
    Common::Messages::EntityGetInfoReq entityGetInfoReq;
    entityGetInfoReq.id = entityId;

    connection.send(entityGetInfoReq);

    std::auto_ptr<Common::Messages::AbstractMessage> shipInfo = connection.receive();
    EXPECT_TRUE(Common::Messages::Id::ShipInfo == shipInfo->getId());

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


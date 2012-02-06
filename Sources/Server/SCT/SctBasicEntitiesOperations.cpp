#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <boost/foreach.hpp>

#include <Common/Thread.hpp>

#include "Core/Component.hpp"
#include "Core/Connection.hpp"
#include "Preconditions.hpp"

using namespace Common::Messages;

class SctBasicEntitiesOperations : public testing::Test 
{
public:
    std::auto_ptr<Common::Messages::AbstractMessage> procedureEntityGetInfo(SCT::Connection & connection, int entityId);
    void procedureEntityChangeCourse(SCT::Connection & connection, int entityId, int x, int y, int z);
};

TEST_F(SctBasicEntitiesOperations, testPlayerEntitiesStatusReq)
{
    SCT::PreconditionPlayerLoggedIn precondition;
    SCT::Connection & connection = precondition.getConnection();

    Common::Messages::PlayerEntitiesStatusReq msg;
    connection.send(msg);

    std::auto_ptr<AbstractMessage> resp = connection.receive();
    EXPECT_TRUE(Common::Messages::Id::PlayerEntitiesStatusResp == resp->getId());
    Common::Messages::PlayerEntitiesStatusResp & playerEntitiesStatusResp = static_cast<Common::Messages::PlayerEntitiesStatusResp &>(*resp);
    ASSERT_EQ(1, playerEntitiesStatusResp.entities.size()); 
    ASSERT_EQ(1, playerEntitiesStatusResp.entities[0].get<0>());
}

TEST_F(SctBasicEntitiesOperations, testEntityChangeCourseReq)
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

    Common::Thread::wait(2.0);

    std::auto_ptr<Common::Messages::AbstractMessage> entityGetInfoRespA2 = procedureEntityGetInfo(connection, 1);
    Common::Messages::ShipInfo & entitiesGetInfoResp2 = dynamic_cast<Common::Messages::ShipInfo&>(*entityGetInfoRespA2);
    EXPECT_TRUE(1 == entitiesGetInfoResp2.player_id);
    EXPECT_TRUE(2 == entitiesGetInfoResp2.x);
    EXPECT_TRUE(1 == entitiesGetInfoResp2.y);
    EXPECT_TRUE(1 == entitiesGetInfoResp2.z);
}

TEST_F(SctBasicEntitiesOperations, ChangeShipCourseAnotherPlayerIsNotified)
{
    std::string dbFile = "SampleDataBase.xml";

	SCT::Component component;
    component.setConfigValue("--database.provider", "xml");
    component.setConfigValue("--database.xml.filename", dbFile);
    component.start();

    // log in user1
    boost::shared_ptr<SCT::Connection> connection1 = component.createConnection();

    UserAuthorizationReq userAuthorizationReq1;
    userAuthorizationReq1.login = "user1";
    userAuthorizationReq1.password = "password";
    connection1->send(userAuthorizationReq1);

    std::auto_ptr<AbstractMessage> userAuthorizationResp1 = connection1->receive();
    ASSERT_TRUE(dynamic_cast<UserAuthorizationResp&>(*userAuthorizationResp1).success);

    // log in user2
    boost::shared_ptr<SCT::Connection> connection2 = component.createConnection();

    UserAuthorizationReq userAuthorizationReq2;
    userAuthorizationReq2.login = "user2";
    userAuthorizationReq2.password = "password";
    connection2->send(userAuthorizationReq2);

    std::auto_ptr<AbstractMessage> userAuthorizationResp2 = connection2->receive();
    ASSERT_TRUE(dynamic_cast<UserAuthorizationResp&>(*userAuthorizationResp2).success);

    procedureEntityChangeCourse(*connection1, 1, 2, 1, 1);

    // second player gets notified
    std::auto_ptr<AbstractMessage> entityChangeCourse2 = connection2->receive();
}

std::auto_ptr<Common::Messages::AbstractMessage> SctBasicEntitiesOperations::procedureEntityGetInfo(
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

void SctBasicEntitiesOperations::procedureEntityChangeCourse(
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


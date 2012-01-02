#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <boost/foreach.hpp>

#include <Common/Thread.hpp>

#include "Core/Component.hpp"
#include "Core/Connection.hpp"
#include "Preconditions.hpp"

class SctBasicEntitiesOperations : public testing::Test 
{
public:
    void testPlayerEntitiesStatusReq();
    void testEntityChangeCourseReq();

    std::auto_ptr<Common::Messages::AbstractMessage> procedureEntityGetInfo(SCT::Connection & connection, int entityId);
    void procedureEntityChangeCourse(SCT::Connection & connection, int entityId, int x, int y, int z);
};

TEST_F(SctBasicEntitiesOperations, testPlayerEntitiesStatusReq)
{
    using namespace ::Common::Messages;

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
    Common::Messages::EntityGetInfoResp & entitiesGetInfoResp = dynamic_cast<Common::Messages::EntityGetInfoResp&>(*entityGetInfoRespA);
    EXPECT_TRUE(1 == entitiesGetInfoResp.player_id);
    EXPECT_TRUE(1 == entitiesGetInfoResp.x);
    EXPECT_TRUE(1 == entitiesGetInfoResp.y);
    EXPECT_TRUE(1 == entitiesGetInfoResp.z);

    procedureEntityChangeCourse(connection, 1, 2, 1, 1);

    Common::Thread::wait(2.0);

    std::auto_ptr<Common::Messages::AbstractMessage> entityGetInfoRespA2 = procedureEntityGetInfo(connection, 1);
    Common::Messages::EntityGetInfoResp & entitiesGetInfoResp2 = dynamic_cast<Common::Messages::EntityGetInfoResp&>(*entityGetInfoRespA2);
    EXPECT_TRUE(1 == entitiesGetInfoResp2.player_id);
    EXPECT_TRUE(2 == entitiesGetInfoResp2.x);
    EXPECT_TRUE(1 == entitiesGetInfoResp2.y);
    EXPECT_TRUE(1 == entitiesGetInfoResp2.z);
}

std::auto_ptr<Common::Messages::AbstractMessage> SctBasicEntitiesOperations::procedureEntityGetInfo(
    SCT::Connection & connection, 
    int entityId)
{
    Common::Messages::EntityGetInfoReq entityGetInfoReq;
    entityGetInfoReq.id = entityId;

    connection.send(entityGetInfoReq);

    std::auto_ptr<Common::Messages::AbstractMessage> entityGetInfoResp = connection.receive();
    EXPECT_TRUE(Common::Messages::Id::EntityGetInfoResp == entityGetInfoResp->getId());

    return entityGetInfoResp;
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


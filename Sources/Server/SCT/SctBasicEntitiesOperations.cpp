#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <fstream>
#include <iostream>
#include <boost/foreach.hpp>

#include <Common/Thread.hpp>

#include <SCT/Component.hpp>
#include <SCT/Connection.hpp>
#include <SCT/DataBaseUtils.hpp>
#include "Server/SCT/Preconditions.hpp"

class SctBasicEntitiesOperations : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE (SctBasicEntitiesOperations);
    CPPUNIT_TEST(testPlayerEntitiesStatusReq);
    CPPUNIT_TEST(testEntityChangeCourseReq);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp();
    void testPlayerEntitiesStatusReq();
    void testEntityChangeCourseReq();

    std::auto_ptr<Common::Messages::AbstractMessage> procedureEntityGetInfo(SCT::Connection & connection, int entityId);
    void procedureEntityChangeCourse(SCT::Connection & connection, int entityId, int x, int y, int z);
};

CPPUNIT_TEST_SUITE_REGISTRATION (SctBasicEntitiesOperations);

void SctBasicEntitiesOperations::setUp()
{
}

void SctBasicEntitiesOperations::testPlayerEntitiesStatusReq()
{
    using namespace ::Common::Messages;

    SCT::PreconditionPlayerLoggedIn precondition;
    SCT::Connection & connection = precondition.getConnection();

    Common::Messages::PlayerEntitiesStatusReq msg;
    connection.send(msg);

    std::auto_ptr<AbstractMessage> resp = connection.receive();
    CPPUNIT_ASSERT_EQUAL(Common::Messages::Id::PlayerEntitiesStatusResp, resp->getId());
    Common::Messages::PlayerEntitiesStatusResp & playerEntitiesStatusResp = static_cast<Common::Messages::PlayerEntitiesStatusResp &>(*resp);
    CPPUNIT_ASSERT_EQUAL(size_t(1), playerEntitiesStatusResp.entities.size()); 
    CPPUNIT_ASSERT_EQUAL(1, playerEntitiesStatusResp.entities[0].get<0>());
}

void SctBasicEntitiesOperations::testEntityChangeCourseReq()
{
    SCT::PreconditionPlayerLoggedIn precondition;
    SCT::Connection & connection = precondition.getConnection();

    std::auto_ptr<Common::Messages::AbstractMessage> entityGetInfoRespA = procedureEntityGetInfo(connection, 1);
    Common::Messages::EntityGetInfoResp & entitiesGetInfoResp = dynamic_cast<Common::Messages::EntityGetInfoResp&>(*entityGetInfoRespA);
    CPPUNIT_ASSERT_EQUAL(1, entitiesGetInfoResp.player_id);
    CPPUNIT_ASSERT_EQUAL(1, entitiesGetInfoResp.x);
    CPPUNIT_ASSERT_EQUAL(1, entitiesGetInfoResp.y);
    CPPUNIT_ASSERT_EQUAL(1, entitiesGetInfoResp.z);

    procedureEntityChangeCourse(connection, 1, 2, 1, 1);

    Common::Thread::wait(1.5);

    std::auto_ptr<Common::Messages::AbstractMessage> entityGetInfoRespA2 = procedureEntityGetInfo(connection, 1);
    Common::Messages::EntityGetInfoResp & entitiesGetInfoResp2 = dynamic_cast<Common::Messages::EntityGetInfoResp&>(*entityGetInfoRespA2);
    CPPUNIT_ASSERT_EQUAL(1, entitiesGetInfoResp2.player_id);
    CPPUNIT_ASSERT_EQUAL(2, entitiesGetInfoResp2.x);
    CPPUNIT_ASSERT_EQUAL(1, entitiesGetInfoResp2.y);
    CPPUNIT_ASSERT_EQUAL(1, entitiesGetInfoResp2.z);
}

std::auto_ptr<Common::Messages::AbstractMessage> SctBasicEntitiesOperations::procedureEntityGetInfo(
    SCT::Connection & connection, 
    int entityId)
{
    Common::Messages::EntityGetInfoReq entityGetInfoReq;
    entityGetInfoReq.id = entityId;

    connection.send(entityGetInfoReq);

    std::auto_ptr<Common::Messages::AbstractMessage> entityGetInfoResp = connection.receive();
    CPPUNIT_ASSERT_EQUAL(Common::Messages::Id::EntityGetInfoResp, entityGetInfoResp->getId());

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

//TODO: FTCs


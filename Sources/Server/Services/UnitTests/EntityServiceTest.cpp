#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Common/Messages/Messages.hpp"

#include "DataBase/DataBase.hpp"
#include "Services/EntityService.hpp"
#include "UnitTests/ConnectionStub.hpp"
#include "Common/Game/UnitTests/RustedTimeStub.hpp"

using namespace Server;
using namespace Common;

class EntityServiceTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE (EntityServiceTest);
    CPPUNIT_TEST(testEntityGetInfo);
    CPPUNIT_TEST_SUITE_END ();

public:
    void testEntityGetInfo();
};

CPPUNIT_TEST_SUITE_REGISTRATION (EntityServiceTest);

void EntityServiceTest::testEntityGetInfo()
{
    DataBase::DataBase db;

    // create some entity to work with
    DataBase::DataBaseNode & entitiesNode = db.getRoot().createChild("entities");
    DataBase::DataBaseNode & entityNode = entitiesNode.createChild("1");
    entityNode.setValue("id", 1);
    entityNode.setValue("player_id", 1);
    entityNode.setValue("x", 1);
    entityNode.setValue("y", 2);
    entityNode.setValue("z", 3);

    // create container and service
    RustedTimeStub time;
    Common::Game::EntityContainer entities(db, time);
    Server::Services::EntityService service(entities);

    // connection stub
    ConnectionStub connection;

    Common::Messages::EntityGetInfoReq getInfoReq;
    getInfoReq.id = 1;

    service.messageReceived(connection, getInfoReq);

    CPPUNIT_ASSERT_EQUAL(size_t(1), connection.getReceivedCount());
    std::auto_ptr<Common::Messages::EntityGetInfoResp> getInfoResp = connection.getReceivedMessage<Common::Messages::EntityGetInfoResp>(0);
    CPPUNIT_ASSERT_EQUAL(1, getInfoResp->id);
    CPPUNIT_ASSERT_EQUAL(1, getInfoResp->x);
    CPPUNIT_ASSERT_EQUAL(2, getInfoResp->y);
    CPPUNIT_ASSERT_EQUAL(3, getInfoResp->z);
}


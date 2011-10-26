#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <fstream>
#include <iostream>
#include <boost/foreach.hpp>

#include <Common/Thread.hpp>

#include <SCT/Component.hpp>
#include <SCT/Connection.hpp>
#include "Server/SCT/DataBaseUtils.hpp"
#include "Server/SCT/Preconditions.hpp"

class SctStaticObjects : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE (SctStaticObjects);
    CPPUNIT_TEST(testStaticObjectsStatusReq);
    CPPUNIT_TEST(testStaticObjectInfoReq);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp();
    void testStaticObjectsStatusReq();
    void testStaticObjectInfoReq();
};

CPPUNIT_TEST_SUITE_REGISTRATION (SctStaticObjects);

void SctStaticObjects::setUp()
{
}

void SctStaticObjects::testStaticObjectsStatusReq()
{
    SCT::PreconditionPlayerLoggedIn precondition;
    SCT::Connection & connection = precondition.getConnection();

    Common::Messages::StaticObjectStatusReq staticObjectStatusReq;
    connection.send(staticObjectStatusReq);
    std::auto_ptr<Common::Messages::AbstractMessage> resp = connection.receive();
    CPPUNIT_ASSERT_EQUAL(Common::Messages::Id::StaticObjectStatusResp, resp->getId());

    Common::Messages::StaticObjectStatusResp & staticObjectStatusResp = dynamic_cast<Common::Messages::StaticObjectStatusResp&>(*resp);
    CPPUNIT_ASSERT_EQUAL(size_t(1), staticObjectStatusResp.objects.size());
    CPPUNIT_ASSERT_EQUAL(1, staticObjectStatusResp.objects[0].get<0>());
}

void SctStaticObjects::testStaticObjectInfoReq()
{
    SCT::PreconditionPlayerLoggedIn precondition;
    SCT::Connection & connection = precondition.getConnection();

    Common::Messages::StaticObjectInfoReq staticObjectInfoReq;
    staticObjectInfoReq.staticObjectId = 1;
    connection.send(staticObjectInfoReq);
    std::auto_ptr<Common::Messages::AbstractMessage> resp = connection.receive();
    CPPUNIT_ASSERT_EQUAL(Common::Messages::Id::StaticObjectInfoResp, resp->getId());

    Common::Messages::StaticObjectInfoResp & staticObjectInfoResp = dynamic_cast<Common::Messages::StaticObjectInfoResp&>(*resp);
    CPPUNIT_ASSERT_EQUAL(1, staticObjectInfoResp.staticObjectId);
    CPPUNIT_ASSERT_EQUAL(100, staticObjectInfoResp.x);
    CPPUNIT_ASSERT_EQUAL(100, staticObjectInfoResp.y);
    CPPUNIT_ASSERT_EQUAL(100, staticObjectInfoResp.z);
}


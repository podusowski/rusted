#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <iostream>
#include <boost/foreach.hpp>

#include <SCT/Component.hpp>
#include <SCT/Connection.hpp>
#include <SCT/DataBaseUtils.hpp>

class SctServerVersion : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE (SctServerVersion);
	CPPUNIT_TEST (testServerVersion);
	CPPUNIT_TEST_SUITE_END ();

public:

protected:
	void testServerVersion();
};

CPPUNIT_TEST_SUITE_REGISTRATION (SctServerVersion);

void SctServerVersion::testServerVersion()
{
	using namespace ::Common::Messages;

	SCT::Component component;
    component.start();
	SCT::Connection & connection = component.createConnection();

	Common::Messages::ServerVersionReq msg;
	connection.send(msg);

	std::auto_ptr<AbstractMessage> resp = connection.receive();
	CPPUNIT_ASSERT_EQUAL(Common::Messages::Id::ServerVersionResp, resp->getId());
}


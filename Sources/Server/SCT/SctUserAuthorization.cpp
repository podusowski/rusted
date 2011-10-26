#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <iostream>
#include <boost/foreach.hpp>

#include <SCT/Component.hpp>
#include <SCT/Connection.hpp>
#include <SCT/DataBaseUtils.hpp>

class SctUserAuthorization : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE (SctUserAuthorization);
	CPPUNIT_TEST (testAuthorizeUser);
	CPPUNIT_TEST_SUITE_END ();

public:

protected:
	void testAuthorizeUser();
};

CPPUNIT_TEST_SUITE_REGISTRATION (SctUserAuthorization);

void SctUserAuthorization::testAuthorizeUser()
{
	using namespace ::Common::Messages;

    SCT::DataBaseUtils dbUtils;
    std::string dbFile = dbUtils.createOnePlayerWithOneEntity();

	::SCT::Component component;
    component.setConfigValue("--database.provider", "xml");
    component.setConfigValue("--database.xml.filename", dbFile);
    component.start();
	::SCT::Connection & connection = component.createConnection();

	// authorize user
	{
		Common::Messages::UserAuthorizationReq msg;
		msg.login = "user1";
		msg.password = "password";
		connection.send(msg);

		std::auto_ptr<AbstractMessage> resp = connection.receive();
		CPPUNIT_ASSERT_EQUAL(Common::Messages::Id::UserAuthorizationResp, resp->getId());
		Common::Messages::UserAuthorizationResp & userAuthorizationResp =
			dynamic_cast<Common::Messages::UserAuthorizationResp&>(*(resp.get()));

		CPPUNIT_ASSERT_EQUAL(true, userAuthorizationResp.success);
		CPPUNIT_ASSERT_EQUAL(1, userAuthorizationResp.player_id);
	}

	// check player's resources
	{
		::Common::Messages::PlayerResourcesStatusReq resourcesStatusReq;
		connection.send(resourcesStatusReq);

		std::auto_ptr<AbstractMessage> resp = connection.receive();
		CPPUNIT_ASSERT_EQUAL(Common::Messages::Id::PlayerResourcesStatusResp, resp->getId());
		Common::Messages::PlayerResourcesStatusResp & playerResourcesStatus =
			dynamic_cast<Common::Messages::PlayerResourcesStatusResp&>(*(resp.get()));

		CPPUNIT_ASSERT_EQUAL(0xf00d, playerResourcesStatus.carbon);
		CPPUNIT_ASSERT_EQUAL(0xf00d, playerResourcesStatus.uranium);
		CPPUNIT_ASSERT_EQUAL(0xf00d, playerResourcesStatus.credits);
	}
}

#include <gtest/gtest.h>

#include <iostream>
#include <boost/foreach.hpp>

#include <Core/Component.hpp>
#include <Core/Connection.hpp>

TEST(SctUserAuthorization, testAuthorizeUser)
{
	using namespace ::Common::Messages;

    std::string dbFile = "SampleDataBase.xml";

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
		EXPECT_EQ(Common::Messages::Id::UserAuthorizationResp, resp->getId());
		Common::Messages::UserAuthorizationResp & userAuthorizationResp =
			dynamic_cast<Common::Messages::UserAuthorizationResp&>(*(resp.get()));

		EXPECT_EQ(true, userAuthorizationResp.success);
		EXPECT_EQ(1, userAuthorizationResp.player_id);
	}

	// check player's resources
	{
		::Common::Messages::PlayerResourcesStatusReq resourcesStatusReq;
		connection.send(resourcesStatusReq);

		std::auto_ptr<AbstractMessage> resp = connection.receive();
		EXPECT_EQ(Common::Messages::Id::PlayerResourcesStatusResp, resp->getId());
		Common::Messages::PlayerResourcesStatusResp & playerResourcesStatus =
			dynamic_cast<Common::Messages::PlayerResourcesStatusResp&>(*(resp.get()));

		EXPECT_EQ(0xf00d, playerResourcesStatus.carbon);
		EXPECT_EQ(0xf00d, playerResourcesStatus.uranium);
		EXPECT_EQ(0xf00d, playerResourcesStatus.credits);
	}
}

#include <gtest/gtest.h>

#include <iostream>
#include <boost/foreach.hpp>

#include "Core/Component.hpp"
#include "Core/Connection.hpp"
#include "Preconditions.hpp"
#include "UserFunctions.hpp"

TEST(UserSct, Authorize)
{
	using namespace ::Common::Messages;

    std::string dbFile = "SampleDataBase.xml";

	SCT::Component component;
    component.setConfigValue("--database.provider", "xml");
    component.setConfigValue("--database.xml.filename", dbFile);
    component.start();
	boost::shared_ptr<SCT::Connection> connection = component.createConnection();

	// authorize user
	{
		Common::Messages::UserAuthorizationReq msg;
		msg.login = "user1";
		msg.password = "password";
		connection->send(msg);

		std::auto_ptr<AbstractMessage> resp = connection->receive();
		EXPECT_EQ(Common::Messages::Id::UserAuthorizationResp, resp->getId());
		Common::Messages::UserAuthorizationResp & userAuthorizationResp =
			dynamic_cast<Common::Messages::UserAuthorizationResp&>(*(resp.get()));

		EXPECT_EQ(true, userAuthorizationResp.success);
		EXPECT_EQ(1, userAuthorizationResp.player_id);
	}

	// check player's resources
	{
		::Common::Messages::PlayerResourcesStatusReq resourcesStatusReq;
		connection->send(resourcesStatusReq);

		std::auto_ptr<AbstractMessage> resp = connection->receive();
		EXPECT_EQ(Common::Messages::Id::PlayerResourcesStatusResp, resp->getId());
		Common::Messages::PlayerResourcesStatusResp & playerResourcesStatus =
			dynamic_cast<Common::Messages::PlayerResourcesStatusResp&>(*(resp.get()));

		EXPECT_EQ(0xf00d, playerResourcesStatus.carbon);
		EXPECT_EQ(0xf00d, playerResourcesStatus.uranium);
		EXPECT_EQ(0xf00d, playerResourcesStatus.credits);
	}
}

TEST(UserSct, TwoUsersEntitiesStatusReq)
{
    std::string dbFile = "SampleDataBase.xml";

	SCT::Component component;
    component.setConfigValue("--database.provider", "xml");
    component.setConfigValue("--database.xml.filename", dbFile);
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 
    boost::shared_ptr<SCT::Connection> connection2 = authorizeUser(component, "user2", "password"); 

    // first player
    {
        Common::Messages::PlayerEntitiesStatusReq msg;
        connection1->send(msg);

        std::auto_ptr<AbstractMessage> resp = connection1->receive();
        EXPECT_TRUE(Common::Messages::Id::PlayerEntitiesStatusResp == resp->getId());
        Common::Messages::PlayerEntitiesStatusResp & playerEntitiesStatusResp = static_cast<Common::Messages::PlayerEntitiesStatusResp &>(*resp);
        ASSERT_EQ(1, playerEntitiesStatusResp.entities.size()); 
        ASSERT_EQ(1, playerEntitiesStatusResp.entities[0].get<0>());
    }

    // second player
    {
        Common::Messages::PlayerEntitiesStatusReq msg;
        connection2->send(msg);

        std::auto_ptr<AbstractMessage> resp = connection2->receive();
        EXPECT_TRUE(Common::Messages::Id::PlayerEntitiesStatusResp == resp->getId());
        Common::Messages::PlayerEntitiesStatusResp & playerEntitiesStatusResp = static_cast<Common::Messages::PlayerEntitiesStatusResp &>(*resp);
        ASSERT_EQ(2, playerEntitiesStatusResp.entities.size()); 
    }
}

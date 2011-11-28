#include <gtest/gtest.h>

#include <iostream>
#include <boost/foreach.hpp>

#include <SCT/Component.hpp>
#include <SCT/Connection.hpp>
#include <SCT/DataBaseUtils.hpp>

TEST(SctServerVersion, testServerVersion)
{
	using namespace ::Common::Messages;

	SCT::Component component;
    component.start();
	SCT::Connection & connection = component.createConnection();

	Common::Messages::ServerVersionReq msg;
	connection.send(msg);

	std::auto_ptr<AbstractMessage> resp = connection.receive();
	EXPECT_TRUE(Common::Messages::Id::ServerVersionResp == resp->getId());
}


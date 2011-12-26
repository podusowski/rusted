#include <gtest/gtest.h>

#include <iostream>
#include <boost/foreach.hpp>

#include <Core/Component.hpp>
#include <Core/Connection.hpp>
#include <Core/DataBaseUtils.hpp>

TEST(SctServerVersion, testServerVersion)
{
	using namespace ::Common::Messages;

    std::string dbFile = "SampleDataBase.xml";

	SCT::Component component;
    component.setConfigValue("--database.provider", "xml");
    component.setConfigValue("--database.xml.filename", dbFile);
    component.start();
	SCT::Connection & connection = component.createConnection();

	Common::Messages::ServerVersionReq msg;
	connection.send(msg);

	std::auto_ptr<AbstractMessage> resp = connection.receive();
	EXPECT_TRUE(Common::Messages::Id::ServerVersionResp == resp->getId());
}


#include <gtest/gtest.h>

#include <iostream>

#include "Core/Component.hpp"
#include "Core/Connection.hpp"

TEST(ServerVersionSct, testServerVersion)
{
	using namespace Common::Messages;

    std::string dbFile = "SampleDataBase.xml";

	SCT::Component component;
    component.setConfigValue("--database.provider", "xml");
    component.setConfigValue("--database.xml.filename", dbFile);
    component.start();
	boost::shared_ptr<SCT::Connection> connection = component.createConnection();

	Common::Messages::ServerVersionReq msg;
	connection->send(msg);

	connection->receive<Common::Messages::ServerVersionResp>();
}

TEST(ServerVersionSct, FewConnections)
{
	using namespace Common::Messages;

    std::string dbFile = "SampleDataBase.xml";

	SCT::Component component;
    component.setConfigValue("--database.provider", "xml");
    component.setConfigValue("--database.xml.filename", dbFile);
    component.start();

    for (int i = 0; i < 5; i++)
    {
        boost::shared_ptr<SCT::Connection> connection = component.createConnection();

        Common::Messages::ServerVersionReq msg;
        connection->send(msg);
        connection->receive<Common::Messages::ServerVersionResp>();
	}
}


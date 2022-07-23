#include <gtest/gtest.h>

#include <iostream>

#include "Core/Component.hpp"
#include "Core/Connection.hpp"

TEST(ServerVersionSct, testServerVersion)
{
    using namespace Common::Messages;

    auto component = SCT::make_default_component();
    component.start();

    std::shared_ptr<SCT::Connection> connection = component.createConnection();

    Common::Messages::ServerVersionReq msg;
    connection->send(msg);

    connection->receive<Common::Messages::ServerVersionResp>();
}

TEST(ServerVersionSct, FewConnections)
{
    using namespace Common::Messages;

    auto component = SCT::make_default_component();
    component.start();

    for (int i = 0; i < 5; i++)
    {
        std::shared_ptr<SCT::Connection> connection = component.createConnection();

        Common::Messages::ServerVersionReq msg;
        connection->send(msg);
        connection->receive<Common::Messages::ServerVersionResp>();
    }
}


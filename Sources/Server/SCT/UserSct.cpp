#include <gtest/gtest.h>

#include "Core/Component.hpp"
#include "Core/Connection.hpp"
#include "UserFunctions.hpp"

TEST(UserSct, Authorize)
{
    using namespace ::Common::Messages;

    auto component = SCT::make_default_component();
    component.start();

    std::shared_ptr<SCT::Connection> connection = component.createConnection();

    Common::Messages::UserAuthorizationReq msg;
    msg.login = "user1";
    msg.password = "password";
    connection->send(msg);

    auto resp = connection->receive<Common::Messages::UserAuthorizationResp>();
    EXPECT_EQ(true, resp->success);
    EXPECT_EQ(1, resp->player_id);
}

TEST(UserSct, InvalidPassword)
{
    auto component = SCT::make_default_component();
    component.start();

    std::shared_ptr<SCT::Connection> connection = component.createConnection();

    Common::Messages::UserAuthorizationReq userAuthorizationReq;
    userAuthorizationReq.login = "user1";
    userAuthorizationReq.password = "some_invalid_password";
    connection->send(userAuthorizationReq);

    auto userAuthorizationResp = connection->receive<Common::Messages::UserAuthorizationResp>();
    EXPECT_FALSE(userAuthorizationResp->success);
}

TEST(UserSct, TwoUsersEntitiesStatusReq)
{
    auto component = SCT::make_default_component();
    component.start();

    std::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 
    std::shared_ptr<SCT::Connection> connection2 = authorizeUser(component, "user2", "password"); 

    // first player
    {
        Common::Messages::FetchPlayerShips msg;
        connection1->send(msg);

        auto playerShips = connection1->receive<Common::Messages::PlayerShips>();
        ASSERT_EQ(1, playerShips->ships.size()); 
        EXPECT_EQ(1, playerShips->ships[0].id);
    }

    // second player
    {
        Common::Messages::FetchPlayerShips msg;
        connection2->send(msg);

        auto playerShips = connection2->receive<Common::Messages::PlayerShips>();
        ASSERT_EQ(3, playerShips->ships.size()); 
    }
}


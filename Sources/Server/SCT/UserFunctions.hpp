#pragma once

#include <boost/shared_ptr.hpp>
#include <gtest/gtest.h>

#include "Core/Component.hpp"
#include "Core/Connection.hpp"

namespace SCT
{

inline boost::shared_ptr<SCT::Connection> authorizeUser(SCT::Component & component, const std::string & login, const std::string & password)
{
    using namespace Common::Messages;

    boost::shared_ptr<SCT::Connection> connection = component.createConnection();

    UserAuthorizationReq userAuthorizationReq;
    userAuthorizationReq.login = login;
    userAuthorizationReq.password = password;
    connection->send(userAuthorizationReq);

    boost::shared_ptr<AbstractMessage> userAuthorizationResp = connection->receive();
    EXPECT_TRUE(dynamic_cast<UserAuthorizationResp&>(*userAuthorizationResp).success);

    return connection;
}

}

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

    auto userAuthorizationResp = connection->receive<UserAuthorizationResp>();
    EXPECT_TRUE(userAuthorizationResp->success);

    return connection;
}

}

#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <boost/foreach.hpp>

#include <Common/Thread.hpp>

#include <Core/Component.hpp>
#include <Core/Connection.hpp>
#include "Core/DataBaseUtils.hpp"
#include "Preconditions.hpp"

TEST(SctStaticObjects, testStaticObjectsStatusReq)
{
    SCT::PreconditionPlayerLoggedIn precondition;
    SCT::Connection & connection = precondition.getConnection();

    Common::Messages::StaticObjectStatusReq staticObjectStatusReq;
    connection.send(staticObjectStatusReq);
    std::auto_ptr<Common::Messages::AbstractMessage> resp = connection.receive();
    EXPECT_EQ(Common::Messages::Id::StaticObjectStatusResp, resp->getId());

    Common::Messages::StaticObjectStatusResp & staticObjectStatusResp = dynamic_cast<Common::Messages::StaticObjectStatusResp&>(*resp);
    EXPECT_EQ(size_t(1), staticObjectStatusResp.objects.size());
    EXPECT_EQ(1, staticObjectStatusResp.objects[0].get<0>());
}

TEST(SctStaticObjects, testStaticObjectInfoReq)
{
    SCT::PreconditionPlayerLoggedIn precondition;
    SCT::Connection & connection = precondition.getConnection();

    Common::Messages::StaticObjectInfoReq staticObjectInfoReq;
    staticObjectInfoReq.staticObjectId = 1;
    connection.send(staticObjectInfoReq);
    std::auto_ptr<Common::Messages::AbstractMessage> resp = connection.receive();
    EXPECT_EQ(Common::Messages::Id::StaticObjectInfoResp, resp->getId());

    Common::Messages::StaticObjectInfoResp & staticObjectInfoResp = dynamic_cast<Common::Messages::StaticObjectInfoResp&>(*resp);
    EXPECT_EQ(1, staticObjectInfoResp.staticObjectId);
    EXPECT_EQ(100, staticObjectInfoResp.x);
    EXPECT_EQ(100, staticObjectInfoResp.y);
    EXPECT_EQ(100, staticObjectInfoResp.z);
}


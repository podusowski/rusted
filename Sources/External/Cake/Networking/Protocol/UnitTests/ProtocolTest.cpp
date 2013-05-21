#include <gtest/gtest.h>

#include "SampleProtocol.hpp"

#include "Diagnostics/Logger.hpp"

#include "Networking/Protocol/CharVectorReadBuffer.hpp"
#include "Networking/Protocol/CharVectorWriteBuffer.hpp"

using namespace Cake::Networking::Protocol;

TEST(ProtocolTest, SimpleParameters)
{
    Cake::Networking::Protocol::SimpleParameters m1;
    m1.integer = 1;
    m1.string = "string";

    SimpleStruct s1;
    s1.integer = 2;
    m1.list.push_back(s1);

    SimpleStruct s2;
    s2.integer = 3;

    ComplexStruct c1;
    c1.list.push_back(s2);
    c1.string = "string";
    m1.complexList.push_back(c1);

    std::vector<char> raw;
    CharVectorWriteBuffer writeBuffer(raw);
    m1.serialize(writeBuffer);

    CharVectorReadBuffer readBuffer(raw);
    boost::shared_ptr<AbstractMessage> abstract = MessageFactory::create(readBuffer);

    auto & m2 = dynamic_cast<SimpleParameters&>(*abstract);

    LOG_INFO << m2;

    EXPECT_EQ(m1.integer, m2.integer);
    EXPECT_EQ(m1.string, m2.string);

    ASSERT_EQ(1, m2.list.size());
    EXPECT_EQ(s1.integer, m2.list[0].integer);

    ASSERT_EQ(1, m2.complexList.size());
    ASSERT_EQ(1, m2.complexList[0].list.size());
    EXPECT_EQ(s2.integer, m2.complexList[0].list[0].integer);
    EXPECT_EQ("string", m2.complexList[0].string);
}


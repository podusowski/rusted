#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SampleProtocol.hpp"

#include "Diagnostics/Logger.hpp"

#include "Networking/Protocol/CharVectorReadBuffer.hpp"
#include "Networking/Protocol/CharVectorWriteBuffer.hpp"

using namespace testing;
using namespace Cake::Networking::Protocol;

TEST(ProtocolTest, Constructing)
{
    SimpleParameters params1;
    SimpleParameters params2(1, "", 1.0, {}, {});
}

TEST(ProtocolTest, SimpleParameters)
{
    SimpleParameters m1;
    m1.integer = 1;
    m1.string = "string";
    m1.real = 1.1234;

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
    std::shared_ptr<AbstractMessage> abstract = MessageFactory::create(readBuffer);

    auto & m2 = dynamic_cast<SimpleParameters&>(*abstract);

    LOG_INFO << m2;

    EXPECT_EQ(m1.integer, m2.integer);
    EXPECT_EQ(m1.string, m2.string);
    EXPECT_FLOAT_EQ(m1.real, m2.real);

    ASSERT_EQ(1u, m2.list.size());
    EXPECT_EQ(s1.integer, m2.list[0].integer);

    ASSERT_EQ(1u, m2.complexList.size());
    ASSERT_EQ(1u, m2.complexList[0].list.size());
    EXPECT_EQ(s2.integer, m2.complexList[0].list[0].integer);
    EXPECT_EQ("string", m2.complexList[0].string);
}

class SampleServiceMock
{
public:
    MOCK_METHOD1(handle, void(const SimpleParameters &));
    MOCK_METHOD1(handle, void(const AbstractMessage &));
};

TEST(ProtocolTest, HandlerCaller)
{
    SimpleParameters m1;
    m1.integer = 0;
    m1.real = 1.0;

    std::vector<char> raw;
    CharVectorWriteBuffer writeBuffer(raw);
    m1.serialize(writeBuffer);

    CharVectorReadBuffer readBuffer(raw);
    std::shared_ptr<AbstractMessage> abstract = MessageFactory::create(readBuffer);

    SampleServiceMock serviceMock;

    EXPECT_CALL(serviceMock, handle(An<const SimpleParameters &>())).Times(1);

    HandlerCaller0<SampleServiceMock> caller(serviceMock);
    caller.call(*abstract);
}

TEST(ProtocolTest, FcDecoder)
{
    auto msg = MessageFactory::create("SimpleMessage(1)");
    auto simple = std::dynamic_pointer_cast<SimpleMessage>(msg);

    EXPECT_EQ(Id::SimpleMessage, msg->getId());
    EXPECT_EQ(1, simple->integer);
}

TEST(ProtocolTest, Fc_NoParameterMessage)
{
    auto msg = MessageFactory::create("NoParameterMessage()\n");
    auto simple = std::dynamic_pointer_cast<NoParameterMessage>(msg);

    EXPECT_EQ(Id::NoParameterMessage, msg->getId());
}

TEST(ProtocolTest, Fc_UnknownMessage)
{
    EXPECT_ANY_THROW(MessageFactory::create("Garbage()"));
}


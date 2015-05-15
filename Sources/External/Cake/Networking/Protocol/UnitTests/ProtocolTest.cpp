#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SampleProtocol.hpp"
#include "SampleProtocol2.hpp"

#include "Diagnostics/Logger.hpp"

#include "Networking/Protocol/CharVectorReadBuffer.hpp"
#include "Networking/Protocol/CharVectorWriteBuffer.hpp"

using namespace testing;
using namespace Cake::Networking::Protocol;

TEST(ProtocolTest, Constructing)
{
    using namespace Old;
    SimpleParameters params1;
    SimpleParameters params2(1, "", 1.0, {}, {});

    EXPECT_EQ(1, params2.integer);
    EXPECT_EQ("", params2.string);
}

TEST(ProtocolTest, Comparison)
{
    {
        using namespace Old;

        SimpleParameters params1(1, "", 1.0, {}, {});
        SimpleParameters params2(2, "", 1.0, {}, {});
        SimpleParameters params3(1, "a", 1.0, {}, {});
        SimpleParameters params4(1, "", 2.0, {}, {});

        SimpleStruct simpleStruct1(1);
        SimpleParameters params5(1, "", 1.0, {simpleStruct1}, {});

        EXPECT_EQ(params1, params1);
        EXPECT_NE(params1, params2);
        EXPECT_NE(params1, params3);
        EXPECT_NE(params1, params4);
        EXPECT_NE(params1, params5);
    }

    {
        using namespace New;

        SimpleParameters params;
        params.integer = 1;
        params.string = "hello";
        params.real = 1.2;

        const SimpleParameters copy = params;
        const SimpleParameters empty;

        LOG_DEBUG << params;

        EXPECT_EQ(params, copy);
        EXPECT_NE(params, empty);
        EXPECT_NE(copy, empty);
    }
}

namespace
{

auto fullEncode(const ICodableStructure & structure) -> Cake::Networking::Bytes
{
    Integer id = structure.id();
    auto ret = id.encode();
    ret.extend(structure.encode());
    return ret;
}

auto fullDecode(const Cake::Networking::Bytes & bytes) -> std::unique_ptr<ICodableStructure>
{
    const auto header = bytes.fromTo(0, 4);
    auto structure = New::decode(header);

    LOG_DEBUG << "Decoding " << structure->str();

    auto rest = bytes.fromTo(4, bytes.size());
    auto neededSize = structure->decode(Cake::Networking::Bytes{});
    while (neededSize > 0)
    {
        LOG_DEBUG << "Still need " << neededSize;
        auto splice = rest.fromTo(0, neededSize);
        rest = rest.fromTo(neededSize, rest.size());
        neededSize = structure->decode(splice);
    }

    return structure;
}

} // namespace

TEST(ProtocolTest, SimpleParametersGetsEncodedAndDecoded)
{
    {
        using namespace Old;
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

    {
        using namespace New;
        SimpleParameters m1;
        m1.integer = 1;
        m1.string = "string";
        m1.real = 1.1234;

        SimpleStruct s1;
        s1.integer = 2;
        m1.list = {s1};

        SimpleStruct s2;
        s2.integer = 3;

        ComplexStruct c1;
        c1.list = {s2};
        c1.string = "string";
        m1.complexList = {c1};

        auto bytes = fullEncode(m1);
        auto decoded = fullDecode(bytes);
        auto & m2 = dynamic_cast<SimpleParameters&>(*decoded);

        EXPECT_EQ(m1, m2);
    }
}

class SampleServiceMock
{
public:
    MOCK_METHOD1(handle, void(const Old::SimpleParameters &));
    MOCK_METHOD1(handle, void(const Old::AbstractMessage &));
};

TEST(ProtocolTest, HandlerCaller)
{
    using namespace Old;
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
    using namespace Old;
    auto msg = MessageFactory::create("SimpleMessage(1)");
    auto simple = std::dynamic_pointer_cast<SimpleMessage>(msg);

    EXPECT_EQ(Id::SimpleMessage, msg->getId());
    EXPECT_EQ(1, simple->integer);
}

TEST(ProtocolTest, Fc_NoParameterMessage)
{
    using namespace Old;
    auto msg = MessageFactory::create("NoParameterMessage()\n");
    auto simple = std::dynamic_pointer_cast<NoParameterMessage>(msg);

    EXPECT_EQ(Id::NoParameterMessage, msg->getId());
}

TEST(ProtocolTest, Fc_UnknownMessage)
{
    using namespace Old;
    EXPECT_ANY_THROW(MessageFactory::create("Garbage()"));
}


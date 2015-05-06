#include <gtest/gtest.h>

#include "Diagnostics/Logger.hpp"
#include "Networking/Protocol/Primitives.hpp"

using namespace Cake::Networking;
using namespace Cake::Networking::Protocol;

TEST(PrimitivesTest, EncodeBoolean)
{
    Boolean boolean{true};
    EXPECT_EQ(1, Boolean::size);

    auto bytes = boolean.encode();

    ASSERT_EQ(Boolean::size, bytes.size());

    const auto * raw = bytes.raw();
    EXPECT_EQ(0x01, raw[0]);
}

TEST(PrimitivesTest, BooleanIsDecoded)
{
    auto bytes = Bytes{1};
    auto * raw = bytes.raw();
    raw[0] = 0x1;

    Boolean boolean {false};
    const auto bytesLeft = boolean.decode(bytes);

    EXPECT_EQ(0, bytesLeft);
    EXPECT_TRUE(*boolean);
}

TEST(PrimitivesTest, IntegerIsCodedToNetworkBytes)
{
    auto integer = Integer{0xBAADFACE};
    EXPECT_EQ(4, Integer::size);

    auto bytes = integer.encode();
    ASSERT_EQ(Integer::size, bytes.size());

    // network - big-endian
    const auto * raw = bytes.raw();
    EXPECT_EQ(0xBA, raw[0]);
    EXPECT_EQ(0xAD, raw[1]);
    EXPECT_EQ(0xFA, raw[2]);
    EXPECT_EQ(0xCE, raw[3]);
}

TEST(PrimitivesTest, IntegerIsDecodedFromNetworkBytes)
{
    auto bytes = Bytes{4};
    auto * raw = bytes.raw();
    raw[0] = 0x12;
    raw[1] = 0x34;
    raw[2] = 0x56;
    raw[3] = 0x78;

    auto integer = Integer{0};
    const auto bytesLeft = integer.decode(bytes);

    EXPECT_EQ(0, bytesLeft);
    EXPECT_EQ(0x12345678, *integer);
}

namespace
{

void expectCodedIntegerEqualTo(const Bytes bytes, const std::uint32_t expected)
{
    LOG_DEBUG << bytes;

    Integer integer;
    integer.decode(bytes);
    EXPECT_EQ(expected, *integer);
}

} // namespace

TEST(PrimitivesTest, IntegerEncodedAndDecoded)
{
    auto bytes = Integer{1234}.encode();

    expectCodedIntegerEqualTo(bytes, 1234);
}

TEST(PrimitivesTest, RealIsEncodedAndDecoded)
{
    auto real = Real{1.5};
    auto bytes = real.encode();

    auto decoded = Real{};
    decoded.decode(bytes);

    EXPECT_NEAR(*real, *decoded, 0.01);
}

TEST(PrimitivesTest, StringIsEncoded)
{
    const auto string = String{"abc"};
    const auto bytes = string.encode();

    ASSERT_EQ(Integer::size + 3, bytes.size());

    // size of the string
    expectCodedIntegerEqualTo(bytes.fromTo(Integer::size * 0, Integer::size * 1), 3);

    // the string itself
    const auto bytesContainingActualString = bytes.fromTo(Integer::size, bytes.size());
    const auto * raw = bytesContainingActualString.raw();
    EXPECT_EQ('a', raw[0]);
    EXPECT_EQ('b', raw[1]);
    EXPECT_EQ('c', raw[2]);
}

TEST(PrimitivesTest, StringIsDecoded)
{
    String string;

    // initial size
    size_t sizeLeft = string.decode(Bytes{});
    EXPECT_EQ(Integer::size, sizeLeft);

    // size of the string
    auto codedSize = Integer{3}.encode();
    sizeLeft = string.decode(codedSize);
    EXPECT_EQ(3, sizeLeft);

    auto bytes = Bytes{3};
    auto * raw = bytes.raw();
    raw[0] = 'a';
    raw[1] = 'b';
    raw[2] = 'c';

    sizeLeft = string.decode(bytes);
    EXPECT_EQ(0, sizeLeft);
    EXPECT_EQ("abc", *string);
}

TEST(PrimitivesTest, EncodeSequenceOfIntegers)
{
    Sequence<Integer> seq1{Integer{10},
                           Integer{20}};
    auto bytes = seq1.encode();

    expectCodedIntegerEqualTo(bytes.fromTo(Integer::size * 0, Integer::size * 1), 2);
    expectCodedIntegerEqualTo(bytes.fromTo(Integer::size * 1, Integer::size * 2), 10);
    expectCodedIntegerEqualTo(bytes.fromTo(Integer::size * 2, Integer::size * 3), 20);
}

TEST(PrimitivesTest, DecodeSequenceOfIntegers)
{
    Sequence<Integer> seq;

    // initial required size
    size_t sizeLeft = seq.decode(Bytes{});
    EXPECT_EQ(Integer::size, sizeLeft);

    // size of the container gets decoded
    auto codedSize = Integer{2}.encode();
    sizeLeft = seq.decode(codedSize);
    EXPECT_EQ(4, sizeLeft);

    // 1th element
    auto codedInteger = Integer{1}.encode();
    sizeLeft = seq.decode(codedInteger);
    EXPECT_EQ(4, sizeLeft);

    // 2nd element
    codedInteger = Integer{2}.encode();
    sizeLeft = seq.decode(codedInteger);
    EXPECT_EQ(0, sizeLeft);
    EXPECT_EQ((Sequence<Integer>{1, 2}), seq);
}

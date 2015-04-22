#include <gtest/gtest.h>

#include "Networking/Protocol/Types.hpp"

using namespace Cake::Networking;
using namespace Cake::Networking::Protocol;

TEST(TypesTest, IntegerIsCodedToNetworkBytes)
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

TEST(TypesTest, IntegerIsDecodedFromNetworkBytes)
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

TEST(TypesTest, IntegerCodedAndEncoded)
{
    Integer integer1(1234);
    auto bytes = integer1.encode();

    Integer integer2;
    integer2.decode(bytes);

    EXPECT_EQ(1234, *integer2);
}

TEST(TypesTest, SequenceOfIntegersCodedAndEncoded)
{
    Sequence<Integer> seq1{Integer{10},
                           Integer{20}};
    auto bytes = seq1.encode();

    Integer size;
    size.decode(bytes);
    EXPECT_EQ(2, *size);

    Integer item1;
    item1.decode(bytes.fromTo(Integer::size, Integer::size * 2));
//    EXPECT_EQ(10, *item1);
//TODO
}

TEST(TypesTest, EncodeSequenceOfIntegers)
{
    Sequence<Integer> seq;

    // size of the container gets decoded
    auto codedSize = Integer{2}.encode();
    size_t sizeLeft = seq.decode(codedSize);
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

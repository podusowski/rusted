#include <gtest/gtest.h>

#include "Networking/Protocol/Types.hpp"

using namespace Cake::Networking;
using namespace Cake::Networking::Protocol;

TEST(TypesTest, IntegerCodedAndEncoded)
{
    Integer integer1(5);
    auto bytes = integer1.encode();

    Integer integer2;
    size_t sizeLeft = integer2.decode(Bytes{});
    EXPECT_EQ(4, sizeLeft);

    sizeLeft = integer2.decode(bytes);
    EXPECT_EQ(0, sizeLeft);

    EXPECT_EQ(5, *integer2);
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

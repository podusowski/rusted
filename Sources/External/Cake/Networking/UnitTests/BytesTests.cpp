#include "Networking/Bytes.hpp"

#include <gtest/gtest.h>

using namespace Cake::Networking;

TEST(BytesTests, ConstructEmptyBytes)
{
    Bytes bytes;
    EXPECT_EQ(0, bytes.size());
}

TEST(BytesTests, TakeSomeUsingFromTo)
{
    auto bytes = Bytes{3};
    auto * chars = bytes.chars();
    chars[0] = 1;
    chars[1] = 2;
    chars[2] = 3;

    auto firstByte = bytes.fromTo(0, 1);
    EXPECT_EQ(1, firstByte.size());
    EXPECT_EQ(1, firstByte.chars()[0]);

    auto lastTwoBytes = bytes.fromTo(1, 3);
    ASSERT_EQ(2, lastTwoBytes.size());
    EXPECT_EQ(2, lastTwoBytes.chars()[0]);
    EXPECT_EQ(3, lastTwoBytes.chars()[1]);
}

TEST(BytesTests, Extend)
{
    auto bytes = Bytes{1};
    bytes.chars()[0] = 1;

    auto moreBytes = Bytes{1};
    moreBytes.chars()[0] = 2;

    bytes.extend(moreBytes);
    ASSERT_EQ(2, bytes.size());
    EXPECT_EQ(1, bytes.chars()[0]);
    EXPECT_EQ(2, bytes.chars()[1]);
}

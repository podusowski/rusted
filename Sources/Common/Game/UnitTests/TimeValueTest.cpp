#include <gtest/gtest.h>

#include "Common/Game/TimeValue.hpp"

using namespace Common::Game;

TEST(TimeValueTest, Substract1)
{
    ASSERT_EQ(TimeValue(1, 0), TimeValue(2, 0) - TimeValue(1, 0));
    ASSERT_EQ(TimeValue(0, 999), TimeValue(1, 0) - TimeValue(0, 1));
}

TEST(TimeValueTest, LessThan)
{
    EXPECT_TRUE(TimeValue(1, 0) < TimeValue(2, 0));
    EXPECT_TRUE(TimeValue(1, 0) < TimeValue(2, 1));
    EXPECT_TRUE(TimeValue(1, 0) < TimeValue(1, 1));
    EXPECT_FALSE(TimeValue(2, 0) < TimeValue(2, 0));
    EXPECT_FALSE(TimeValue(2, 0) < TimeValue(1, 0));
    EXPECT_FALSE(TimeValue(2, 0) < TimeValue(1, 1));
    EXPECT_FALSE(TimeValue(2, 1) < TimeValue(1, 1));
}


#include <gtest/gtest.h>

#include "Common/Game/TimeValue.hpp"

using namespace Common::Game;

TEST(TimeValueTest, Substract1)
{
    ASSERT_EQ(TimeValue(1, 0), TimeValue(2, 0) - TimeValue(1, 0));
    ASSERT_EQ(TimeValue(0, 999), TimeValue(1, 0) - TimeValue(0, 1));
}

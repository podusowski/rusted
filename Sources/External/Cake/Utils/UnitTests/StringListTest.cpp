#include <gtest/gtest.h>

#include "Utils/StringList.hpp"

using namespace testing;

TEST(StringListTest, Basic)
{
    Cake::Utils::StringList list;
    EXPECT_EQ("", list.toString());
    EXPECT_EQ(0, list.size());

    list.add("a");
    EXPECT_EQ("a", list.toString());
    EXPECT_EQ(1, list.size());

    list.add() << "b" << 1;
    EXPECT_EQ("a\nb1", list.toString());
    EXPECT_EQ(2, list.size());
}


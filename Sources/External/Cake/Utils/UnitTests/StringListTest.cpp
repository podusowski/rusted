#include <gtest/gtest.h>

#include "Utils/StringList.hpp"

using namespace testing;

TEST(StringListTest, Basic)
{
    Cake::Utils::StringList list;
    EXPECT_EQ("", list.toString());

    list.add("a");
    EXPECT_EQ("a", list.toString());

    list.add("b");
    EXPECT_EQ("a\nb", list.toString());
}


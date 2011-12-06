#include <gtest/gtest.h>

#include "System/Shell.hpp"

using namespace testing;
using namespace Common::System;

TEST(ShellTest, SuccessfulReturnCode)
{
    Shell shell;
    ShellResult result = shell.execute("true");

    EXPECT_TRUE(result.getReturnCode() == 0);
}

TEST(ShellTest, ErrorReturnCode)
{
    Shell shell;
    ShellResult result = shell.execute("false");

    EXPECT_TRUE(result.getReturnCode() != 0);
}

TEST(ShellTest, SpecificReturnCode)
{
    Shell shell;
    ShellResult result = shell.execute("bash -c \"exit 2\"");

    EXPECT_TRUE(result.getReturnCode() == 2);
}

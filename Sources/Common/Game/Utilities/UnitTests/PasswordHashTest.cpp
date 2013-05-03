#include <gtest/gtest.h>

#include "Game/Utilities/PasswordHash.hpp"

TEST(PasswordHashTest, Hashing)
{
    Common::Game::Utilities::PasswordHash hash;

    EXPECT_TRUE(hash.generate("test") != hash.generate("tesT"));
    EXPECT_TRUE(hash.generate("a").length() < 100);
    EXPECT_TRUE(hash.generate("same") != "same");
}


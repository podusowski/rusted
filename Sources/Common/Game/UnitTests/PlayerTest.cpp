#include <gtest/gtest.h>

#include "Common/Game/Player.hpp"

TEST(PlayerTest, Id)
{
    Common::Game::Player player;
    EXPECT_ANY_THROW(player.getId());
    player.setId(1);
    EXPECT_EQ(1, player.getId());
}

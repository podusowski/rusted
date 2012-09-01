#include <gtest/gtest.h>

#include "Common/Game/Object/UnitTests/ShipMock.hpp"
#include "Common/Game/Player.hpp"

TEST(PlayerTest, Id)
{
    Common::Game::Player player;
    EXPECT_ANY_THROW(player.getId());
    player.setId(1);
    EXPECT_EQ(1, player.getId());
}

TEST(PlayerTest, FocusedObject)
{
    Common::Game::Object::ShipMock ship;
    Common::Game::Player player;

    EXPECT_ANY_THROW(player.getFocusedObject());
    player.focusObject(ship);
    EXPECT_EQ(&ship, &player.getFocusedObject());
}

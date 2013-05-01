#include <gtest/gtest.h>

#include "Common/UnitTests/AbstractTest.hpp"
#include "Common/Game/Object/UnitTests/ShipMock.hpp"
#include "Common/Game/Player.hpp"

class PlayerTest : public Common::AbstractTest
{
};

TEST_F(PlayerTest, Id)
{
    Common::Game::Player player;
    EXPECT_ANY_THROW(player.getId());
    player.setId(1);
    EXPECT_EQ(1, player.getId());
}

TEST_F(PlayerTest, FocusedObject)
{
    Common::Game::Object::ShipMock ship;
    Common::Game::Player player;

    EXPECT_ANY_THROW(player.getFocusedObject());
    player.focusObject(ship);
    EXPECT_EQ(&ship, &player.getFocusedObject());
}


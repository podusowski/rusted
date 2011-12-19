#include <gtest/gtest.h>

#include "Game/Object/Ship.hpp"
#include "Game/UnitTests/RustedTimeStub.hpp"

TEST(ShipTest, TestMoveByVector)
{
    RustedTimeStub time;
    Common::Game::Object::Ship ship;

    ship.setPosition(Common::Game::Position(0, 0, 0));
}

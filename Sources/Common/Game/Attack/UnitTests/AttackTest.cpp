#include <gtest/gtest.h>

#include "Game/Attack/Attack.hpp"
#include "Game/Object/UnitTests/ShipMock.hpp"

using namespace testing;

TEST(AttackTest, Simple)
{
    Common::Game::Object::ShipMock ship1;
    Common::Game::Attack::Attack attack;

    // doesn't matter if this is called, might be cached or something
    // the thing is what to return if it's called
    ON_CALL(ship1, getIntegrity()).WillByDefault(Return(100));

    EXPECT_CALL(ship1, setIntegrity(90)).Times(1);

    attack.attack(ship1);
}

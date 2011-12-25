#include <gtest/gtest.h>

#include "Cake/DependencyInjection/Registry.hpp"

#include "Game/Object/Ship.hpp"
#include "Game/UnitTests/RustedTimeStub.hpp"

using namespace testing;

TEST(ShipTest, TestMoveByVector)
{
    boost::shared_ptr<Common::Game::IRustedTime> time(new RustedTimeStub);
    Cake::DependencyInjection::forInterface<Common::Game::IRustedTime>().use(time);

    Common::Game::Object::Ship ship;

    ON_CALL(dynamic_cast<RustedTimeStub&>(*time), getSeconds()).WillByDefault(Return(0));

    ship.setPosition(Common::Game::Position(0, 0, 0));
    ship.setCourse(Common::Game::Position(100, 0, 0));

    EXPECT_CALL(dynamic_cast<RustedTimeStub&>(*time), getSeconds()).Times(1).WillOnce(Return(100));

    ASSERT_EQ(Common::Game::Position(100, 0, 0), ship.getPosition());
}

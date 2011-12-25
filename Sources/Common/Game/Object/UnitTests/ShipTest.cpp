#include <gtest/gtest.h>

#include "Cake/DependencyInjection/Registry.hpp"

#include "Game/Object/Ship.hpp"
#include "Game/UnitTests/RustedTimeStub.hpp"

using namespace testing;

class ShipTest : public Test
{
public:
    void SetUp()
    {
        Cake::DependencyInjection::clear();
        m_time.reset(new RustedTimeStub);
        Cake::DependencyInjection::forInterface<Common::Game::IRustedTime>().use(m_time);
    }

    RustedTimeStub & getRustedTimeStub()
    {
        return dynamic_cast<RustedTimeStub &>(*m_time);
    }

private:
    boost::shared_ptr<Common::Game::IRustedTime> m_time;
};

TEST_F(ShipTest, TestMoveByVector)
{
    Common::Game::Object::Ship ship;

    ON_CALL(getRustedTimeStub(), getSeconds()).WillByDefault(Return(0));

    ship.setPosition(Common::Game::Position(0, 0, 0));
    ship.setCourse(Common::Game::Position(100, 0, 0));

    // one call goes for position calculation, 
    // second for setCourse calculation 
    // and another one again for position calculation
    EXPECT_CALL(getRustedTimeStub(), getSeconds()).Times(3)
        .WillOnce(Return(100))
        .WillOnce(Return(100))
        .WillOnce(Return(200));

    ASSERT_EQ(Common::Game::Position(100, 0, 0), ship.getPosition());

    ship.setCourse(Common::Game::Position(100, 100, 0));

    ASSERT_EQ(Common::Game::Position(100, 100, 0), ship.getPosition());
}


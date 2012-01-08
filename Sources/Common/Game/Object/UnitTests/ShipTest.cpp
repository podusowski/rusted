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

    ship.setPosition(Common::Game::Position(0, 0, 0));

    EXPECT_CALL(getRustedTimeStub(), getSeconds()).Times(1).WillOnce(Return(0));
    ship.setCourse(Common::Game::Position(100, 0, 0));
    Mock::VerifyAndClear(&getRustedTimeStub());

    EXPECT_CALL(getRustedTimeStub(), getSeconds()).Times(1).WillOnce(Return(50));
    ASSERT_EQ(Common::Game::Position(50, 0, 0), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());

    EXPECT_CALL(getRustedTimeStub(), getSeconds()).Times(1).WillOnce(Return(100));
    ASSERT_EQ(Common::Game::Position(100, 0, 0), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());

    EXPECT_CALL(getRustedTimeStub(), getSeconds()).Times(1).WillOnce(Return(100));
    ship.setCourse(Common::Game::Position(100, 100, 0));
    Mock::VerifyAndClear(&getRustedTimeStub());

    // still the same place
    EXPECT_CALL(getRustedTimeStub(), getSeconds()).Times(1).WillOnce(Return(100));
    ASSERT_EQ(Common::Game::Position(100, 0, 0), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());

    // should be half the way
    EXPECT_CALL(getRustedTimeStub(), getSeconds()).Times(1).WillOnce(Return(150));
    ASSERT_EQ(Common::Game::Position(100, 50, 0), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());

    EXPECT_CALL(getRustedTimeStub(), getSeconds()).Times(1).WillOnce(Return(200));
    ASSERT_EQ(Common::Game::Position(100, 100, 0), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());
}

TEST_F(ShipTest, MoveToTheSamePosition)
{
    Common::Game::Object::Ship ship;

    ship.setPosition(Common::Game::Position(1, 2, 3));

    EXPECT_CALL(getRustedTimeStub(), getSeconds()).Times(1).WillOnce(Return(100));
    ship.setCourse(Common::Game::Position(1, 2, 3));
    Mock::VerifyAndClear(&getRustedTimeStub());

    EXPECT_CALL(getRustedTimeStub(), getSeconds()).Times(1).WillOnce(Return(100));
    ASSERT_EQ(Common::Game::Position(1, 2, 3), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());
}


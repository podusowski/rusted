#include <gtest/gtest.h>

#include "Cake/DependencyInjection/Registry.hpp"

#include "Game/Object/Ship.hpp"
#include "Game/UnitTests/RustedTimeStub.hpp"
#include "Game/Object/UnitTests/FlightTrajectoryMock.hpp"

using namespace testing;
using namespace Common::Game;

class ShipTest : public Test
{
public:
    void SetUp()
    {
        using namespace Cake::DependencyInjection;

        Cake::DependencyInjection::clear();

        m_time.reset(new RustedTimeStub);
        forInterface<Common::Game::IRustedTime>().use(m_time);

        m_trajectory.reset(new Common::Game::Object::FlightTrajectoryMock);
        forInterface<Common::Game::Object::IFlightTrajectory>().use(m_trajectory);
    }

    RustedTimeStub & getRustedTimeStub()
    {
        return dynamic_cast<RustedTimeStub &>(*m_time);
    }

    Common::Game::Object::FlightTrajectoryMock & getTrajectoryMock()
    {
        return dynamic_cast<Common::Game::Object::FlightTrajectoryMock&>(*m_trajectory);
    }

private:
    boost::shared_ptr<Common::Game::IRustedTime> m_time;
    boost::shared_ptr<Common::Game::Object::IFlightTrajectory> m_trajectory;
};

TEST_F(ShipTest, TestMoveByVector)
{
    Common::Game::Object::Ship ship;

    ship.setPosition(Common::Game::Position(0, 0, 0));

    EXPECT_CALL(getRustedTimeStub(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(0, 0)));
    ship.setCourse(Common::Game::Position(100, 0, 0));
    Mock::VerifyAndClear(&getRustedTimeStub());

    EXPECT_CALL(getRustedTimeStub(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(50, 0)));
    ASSERT_EQ(Common::Game::Position(50, 0, 0), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());

    EXPECT_CALL(getRustedTimeStub(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(100, 0)));
    ASSERT_EQ(Common::Game::Position(100, 0, 0), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());

    EXPECT_CALL(getRustedTimeStub(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(100, 0)));
    ship.setCourse(Common::Game::Position(100, 100, 0));
    Mock::VerifyAndClear(&getRustedTimeStub());

    // still the same place
    EXPECT_CALL(getRustedTimeStub(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(100, 0)));
    ASSERT_EQ(Common::Game::Position(100, 0, 0), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());

    // should be half the way
    EXPECT_CALL(getRustedTimeStub(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(150, 0)));
    ASSERT_EQ(Common::Game::Position(100, 50, 0), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());

    EXPECT_CALL(getRustedTimeStub(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(200, 0)));
    ASSERT_EQ(Common::Game::Position(100, 100, 0), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());
}

TEST_F(ShipTest, HighPrecisionTimer)
{
    Common::Game::Object::Ship ship;

    ship.setPosition(Common::Game::Position(0, 0, 0));
    ship.setSpeed(2);

    EXPECT_CALL(getRustedTimeStub(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(0, 0)));
    ship.setCourse(Common::Game::Position(2, 0, 0));
    Mock::VerifyAndClear(&getRustedTimeStub());

    EXPECT_CALL(getRustedTimeStub(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(0, 500)));
    ASSERT_EQ(Common::Game::Position(1, 0, 0), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());
}

TEST_F(ShipTest, MoveToTheSamePosition)
{
    Common::Game::Object::Ship ship;

    ship.setPosition(Common::Game::Position(1, 2, 3));

    EXPECT_CALL(getRustedTimeStub(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(100, 0)));
    ship.setCourse(Common::Game::Position(1, 2, 3));
    Mock::VerifyAndClear(&getRustedTimeStub());

    EXPECT_CALL(getRustedTimeStub(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(100, 0)));
    ASSERT_EQ(Common::Game::Position(1, 2, 3), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());
}

/* When ship is destroyed somewhere, it should stop moving and stay in place where it
 * was when it happened. 
 */
TEST_F(ShipTest, ShipDestroyedDuringFlight)
{
    Common::Game::Object::Ship ship;

    ship.setPosition(Common::Game::Position(0, 0, 0));

    EXPECT_CALL(getRustedTimeStub(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(0, 0)));
    ship.setCourse(Common::Game::Position(100, 0, 0));
    Mock::VerifyAndClear(&getRustedTimeStub());

    ON_CALL(getRustedTimeStub(), getCurrentTime()).WillByDefault(Return(TimeValue(50, 0)));
    ASSERT_EQ(Common::Game::Position(50, 0, 0), ship.getPosition());
    ship.setIntegrity(0);
    Mock::VerifyAndClear(&getRustedTimeStub());

    // ship should stop

    ON_CALL(getRustedTimeStub(), getCurrentTime()).WillByDefault(Return(TimeValue(100, 0)));
    ASSERT_EQ(Common::Game::Position(50, 0, 0), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());

    // cant fly anymore

    ON_CALL(getRustedTimeStub(), getCurrentTime()).WillByDefault(Return(TimeValue(200, 0)));
    ship.setCourse(Common::Game::Position(300, 0, 0));
    Mock::VerifyAndClear(&getRustedTimeStub());

    ON_CALL(getRustedTimeStub(), getCurrentTime()).WillByDefault(Return(TimeValue(300, 0)));
    ASSERT_EQ(Common::Game::Position(50, 0, 0), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());
}

/* Now, damaging ship doesn't affect anything 
 */
TEST_F(ShipTest, ShipDamagedDuringFlight)
{
    Common::Game::Object::Ship ship;

    ship.setPosition(Common::Game::Position(0, 0, 0));

    EXPECT_CALL(getRustedTimeStub(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(0, 0)));
    ship.setCourse(Common::Game::Position(100, 0, 0));
    Mock::VerifyAndClear(&getRustedTimeStub());

    ON_CALL(getRustedTimeStub(), getCurrentTime()).WillByDefault(Return(TimeValue(50, 0)));
    ASSERT_EQ(Common::Game::Position(50, 0, 0), ship.getPosition());
    ship.setIntegrity(10);
    Mock::VerifyAndClear(&getRustedTimeStub());

    ON_CALL(getRustedTimeStub(), getCurrentTime()).WillByDefault(Return(TimeValue(100, 0)));
    ASSERT_EQ(Common::Game::Position(100, 0, 0), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());
}

/* Yeah, blame me for not doing OOP but we need internal course representation to send it over
 * the network
 */
TEST_F(ShipTest, CourseObjectAccess)
{
    Common::Game::Object::Ship ship;

    ship.setPosition(Common::Game::Position(10, 20, 30));

    ON_CALL(getRustedTimeStub(), getCurrentTime()).WillByDefault(Return(TimeValue(10, 20)));
    ship.setCourse(Common::Game::Position(100, 200, 300));
 
    auto course = ship.getCourse();

    EXPECT_EQ(10, course.start.getX());
    EXPECT_EQ(20, course.start.getY());
    EXPECT_EQ(30, course.start.getZ());

    EXPECT_EQ(100, course.destination.getX());
    EXPECT_EQ(200, course.destination.getY());
    EXPECT_EQ(300, course.destination.getZ());
}

TEST_F(ShipTest, MakeCourseFromCourseObject)
{
    Common::Game::Object::Ship ship;

    ship.setPosition(Common::Game::Position(0, 0, 0));

    Common::Game::Object::Course course;
    course.start.setX(0);
    course.start.setY(0);
    course.start.setZ(0);

    course.destination.setX(100);
    course.destination.setY(0);
    course.destination.setZ(0);

    course.startTime = TimeValue(0, 0);

    ship.setCourse(course);

    ON_CALL(getRustedTimeStub(), getCurrentTime()).WillByDefault(Return(TimeValue(50, 0)));
    ASSERT_EQ(Common::Game::Position(50, 0, 0), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());

    ON_CALL(getRustedTimeStub(), getCurrentTime()).WillByDefault(Return(TimeValue(100, 0)));
    ASSERT_EQ(Common::Game::Position(100, 0, 0), ship.getPosition());
    Mock::VerifyAndClear(&getRustedTimeStub());
}

TEST_F(ShipTest, SetPositionIsResetingTheCourse)
{
    Common::Game::Object::Ship ship;
    ship.setPosition(Common::Game::Position(10, 20, 30));

    ON_CALL(getRustedTimeStub(), getCurrentTime()).WillByDefault(Return(TimeValue(50, 0)));
    auto course = ship.getCourse();

    EXPECT_EQ(10, course.start.getX());
    EXPECT_EQ(20, course.start.getY());
    EXPECT_EQ(30, course.start.getZ());

    EXPECT_EQ(10, course.destination.getX());
    EXPECT_EQ(20, course.destination.getY());
    EXPECT_EQ(30, course.destination.getZ());
}

TEST_F(ShipTest, DefaultIntegrity)
{
    Common::Game::Object::Ship ship;
    EXPECT_EQ(100, ship.getIntegrity());
}

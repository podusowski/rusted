#include <gtest/gtest.h>

#include "Cake/DependencyInjection/Registry.hpp"
#include "Game/UnitTests/RustedTimeStub.hpp"

#include "Common/Game/Object/FlightTrajectory.hpp"

using namespace testing;

using namespace Common::Game;
using namespace Common::Game::Object;

class FlightTrajectoryTest : public Test
{
public:
    FlightTrajectoryTest() : 
        m_time(new RustedTimeStub)
    {
        Cake::DependencyInjection::clear();
        Cake::DependencyInjection::forInterface<IRustedTime>().use(m_time);
    }

    ~FlightTrajectoryTest()
    {
        Cake::DependencyInjection::clear();
    }

    RustedTimeStub & getTimeMock()
    {
        return *dynamic_cast<RustedTimeStub*>(m_time.get());
    }

private:
    boost::shared_ptr<Common::Game::IRustedTime> m_time;
};

TEST_F(FlightTrajectoryTest, FlyByVector)
{
    FlightTrajectory trajectory;

    // position can be changed explicitly, for example when client get information about ship
    trajectory.setPosition(Position(0, 0, 0));

    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(0, 0)));
    trajectory.fly(Position(100, 0, 0));
    Mock::VerifyAndClear(&getTimeMock());

    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(50, 0)));
    ASSERT_EQ(Position(50, 0, 0), trajectory.getPosition());
    Mock::VerifyAndClear(&getTimeMock());

    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(100, 0)));
    ASSERT_EQ(Position(100, 0, 0), trajectory.getPosition());
    Mock::VerifyAndClear(&getTimeMock());

    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(100, 0)));
    trajectory.fly(Position(100, 100, 0));
    Mock::VerifyAndClear(&getTimeMock());

    // still the same place
    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(100, 0)));
    ASSERT_EQ(Position(100, 0, 0), trajectory.getPosition());
    Mock::VerifyAndClear(&getTimeMock());

    // should be half the way
    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(150, 0)));
    ASSERT_EQ(Position(100, 50, 0), trajectory.getPosition());
    Mock::VerifyAndClear(&getTimeMock());

    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(200, 0)));
    ASSERT_EQ(Position(100, 100, 0), trajectory.getPosition());
    Mock::VerifyAndClear(&getTimeMock());
}

TEST_F(FlightTrajectoryTest, HighPrecisionTimer)
{
    FlightTrajectory trajectory;

    trajectory.setPosition(Position(0, 0, 0));
    trajectory.setSpeed(2);

    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(0, 0)));
    trajectory.fly(Position(2, 0, 0));
    Mock::VerifyAndClear(&getTimeMock());

    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(0, 500)));
    ASSERT_EQ(Position(1, 0, 0), trajectory.getPosition());
    Mock::VerifyAndClear(&getTimeMock());
}

TEST_F(FlightTrajectoryTest, MoveToTheSamePosition)
{
    FlightTrajectory trajectory;

    trajectory.setPosition(Position(1, 2, 3));

    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(100, 0)));
    trajectory.fly(Position(1, 2, 3));
    Mock::VerifyAndClear(&getTimeMock());

    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(200, 0)));
    ASSERT_EQ(Position(1, 2, 3), trajectory.getPosition());
    Mock::VerifyAndClear(&getTimeMock());
}

/* When ship is destroyed somewhere, it should stop moving and stay in place where it
 * was when it happened. 
 */
TEST_F(FlightTrajectoryTest, ShipDestroyedDuringFlight)
{
    FlightTrajectory trajectory;

    trajectory.setPosition(Position(0, 0, 0));

    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(0, 0)));
    trajectory.fly(Common::Game::Position(100, 0, 0));
    Mock::VerifyAndClear(&getTimeMock());

    ON_CALL(getTimeMock(), getCurrentTime()).WillByDefault(Return(TimeValue(50, 0)));
    ASSERT_EQ(Position(50, 0, 0), trajectory.getPosition());
    trajectory.stop();
    Mock::VerifyAndClear(&getTimeMock());

    // ship should stop

    ON_CALL(getTimeMock(), getCurrentTime()).WillByDefault(Return(TimeValue(100, 0)));
    ASSERT_EQ(Position(50, 0, 0), trajectory.getPosition());
    Mock::VerifyAndClear(&getTimeMock());
}

/* Yeah, blame me for not doing OOP but we need internal course representation to send it over
 * the network
 *
 * Sort of memento pattern
 */
TEST_F(FlightTrajectoryTest, GetDescription)
{
    FlightTrajectory trajectory;

    trajectory.setPosition(Position(10, 20, 30));

    ON_CALL(getTimeMock(), getCurrentTime()).WillByDefault(Return(TimeValue(10, 20)));
    trajectory.fly(Common::Game::Position(100, 200, 300));
 
    auto description = trajectory.getDescription();

    EXPECT_EQ(10, description.start.getX());
    EXPECT_EQ(20, description.start.getY());
    EXPECT_EQ(30, description.start.getZ());

    EXPECT_EQ(100, description.destination.getX());
    EXPECT_EQ(200, description.destination.getY());
    EXPECT_EQ(300, description.destination.getZ());
}

TEST_F(FlightTrajectoryTest, ApplyDescription)
{
    FlightTrajectory trajectory;

    trajectory.setPosition(Position(0, 0, 0));

    FlightTrajectory::Description description;
    description.start.setX(0);
    description.start.setY(0);
    description.start.setZ(0);

    description.destination.setX(100);
    description.destination.setY(0);
    description.destination.setZ(0);

    description.startTime = TimeValue(0, 0);

    trajectory.applyDescription(description);

    ON_CALL(getTimeMock(), getCurrentTime()).WillByDefault(Return(TimeValue(50, 0)));
    ASSERT_EQ(Position(50, 0, 0), trajectory.getPosition());
    Mock::VerifyAndClear(&getTimeMock());

    ON_CALL(getTimeMock(), getCurrentTime()).WillByDefault(Return(TimeValue(100, 0)));
    ASSERT_EQ(Position(100, 0, 0), trajectory.getPosition());
    Mock::VerifyAndClear(&getTimeMock());
}

TEST_F(FlightTrajectoryTest, SetPositionIsResetingTheCourse)
{
    FlightTrajectory trajectory;
    trajectory.fly(Position(10, 20, 30));

    ON_CALL(getTimeMock(), getCurrentTime()).WillByDefault(Return(TimeValue(50, 0)));
    auto description = trajectory.getDescription();

    EXPECT_EQ(10, description.start.getX());
    EXPECT_EQ(20, description.start.getY());
    EXPECT_EQ(30, description.start.getZ());

    EXPECT_EQ(10, description.destination.getX());
    EXPECT_EQ(20, description.destination.getY());
    EXPECT_EQ(30, description.destination.getZ());
}


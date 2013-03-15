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

    int radianToDegree(float radians)
    {
        const float C_180_DIV_PI = 57.2958;
        return round(radians * C_180_DIV_PI);
    }

    RustedTimeStub & getTimeMock()
    {
        return *dynamic_cast<RustedTimeStub*>(m_time.get());
    }

private:
    boost::shared_ptr<Common::Game::IRustedTime> m_time;
};

TEST_F(FlightTrajectoryTest, HighPrecisionTimer)
{
    FlightTrajectory trajectory;

    trajectory.setPosition(Position(0, 0, 0));
    trajectory.setSpeed(10000);

    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(0, 0)));
    trajectory.fly(Position(0, 0, 10000));
    Mock::VerifyAndClear(&getTimeMock());

    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(0, 500)));
    ASSERT_EQ(Position(0, 0, 5000), trajectory.getPosition());
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

TEST_F(FlightTrajectoryTest, Stop)
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

 /* Sort of memento pattern, we need to have a way to send FlightTrajectory through network
 */
TEST_F(FlightTrajectoryTest, GetDescription)
{
    FlightTrajectory trajectory;

    trajectory.setPosition(Position(10, 20, 30));

    ON_CALL(getTimeMock(), getCurrentTime()).WillByDefault(Return(TimeValue(10, 20)));
    trajectory.fly(Common::Game::Position(100, 200, 300));
 
    auto description = trajectory.getDescription();

    ASSERT_TRUE(description.controlPoints.size() >= 2);

    EXPECT_EQ(10, description.controlPoints[0].getX());
    EXPECT_EQ(20, description.controlPoints[0].getY());
    EXPECT_EQ(30, description.controlPoints[0].getZ());

    size_t last = description.controlPoints.size() - 1;
    EXPECT_EQ(100, description.controlPoints[last].getX());
    EXPECT_EQ(200, description.controlPoints[last].getY());
    EXPECT_EQ(300, description.controlPoints[last].getZ());
}

TEST_F(FlightTrajectoryTest, ApplyDescription)
{
    FlightTrajectory trajectory;

    trajectory.setPosition(Position(0, 0, 0));

    FlightTrajectory::Description description;

    // some random control points, first and last are most important ones
    description.controlPoints.push_back(Common::Game::Position(0, 0, 0));
    description.controlPoints.push_back(Common::Game::Position(10, 0, 0));
    description.controlPoints.push_back(Common::Game::Position(20, 0, 0));
    description.controlPoints.push_back(Common::Game::Position(100, 0, 0));

    description.startTime = TimeValue(0, 0);

    trajectory.applyDescription(description);

    ON_CALL(getTimeMock(), getCurrentTime()).WillByDefault(Return(TimeValue(100, 0)));
    ASSERT_EQ(Position(100, 0, 0), trajectory.getPosition());

    Mock::VerifyAndClear(&getTimeMock());
}

TEST_F(FlightTrajectoryTest, SetPositionResetsTheCourse)
{
    ON_CALL(getTimeMock(), getCurrentTime()).WillByDefault(Return(TimeValue(50, 0)));

    FlightTrajectory trajectory;
    trajectory.setPosition(Position(10, 20, 30));

    auto description = trajectory.getDescription();
    EXPECT_EQ(0, description.controlPoints.size());
}



#include <gtest/gtest.h>

#include "Common/UnitTests/AbstractTest.hpp"
#include "Common/Game/Object/FlightTrajectory.hpp"

using namespace testing;

using namespace Common::Game;
using namespace Common::Game::Object;

class FlightTrajectoryTest : public Common::AbstractTest
{
public:
    FlightTrajectoryTest()
    {
        ON_CALL(getSpline3Mock(), operatorCall(_)).WillByDefault(Return(Common::Math::ISpline3::PointType()));
        ON_CALL(getSpline3Mock(), derivative(_)).WillByDefault(Return(Common::Math::ISpline3::PointType()));
    }

    int radianToDegree(float radians)
    {
        const float C_180_DIV_PI = 57.2958;
        return round(radians * C_180_DIV_PI);
    }
};

TEST_F(FlightTrajectoryTest, Movement)
{
    FlightTrajectory trajectory;

#if 0
    trajectory.setPosition(Position(0, 0, 0));
    trajectory.setSpeed(10000);
    trajectory.setAcceleration(100000);

    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(0, 0)));
    trajectory.fly(Position(0, 0, 10000));
    Mock::VerifyAndClear(&getTimeMock());

    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(0, 500)));
    ASSERT_GT(5000, trajectory.getPosition().getZ());
    Mock::VerifyAndClear(&getTimeMock());
#endif
}

TEST_F(FlightTrajectoryTest, Stop)
{
    FlightTrajectory trajectory;
    trajectory.setPosition(Position(0, 0, 0));

    ON_CALL(getTimeMock(), getCurrentTime()).WillByDefault(Return(TimeValue(0, 0)));

    // by default ships are facing (0,0,1) so it should fly straight line here
    trajectory.fly(Common::Game::Position(0, 0, 10000));

    EXPECT_CALL(getSpline3Mock(), reset()).Times(1);
    trajectory.stop();
}

TEST_F(FlightTrajectoryTest, SetPositionResetsTheCourse)
{
    ON_CALL(getTimeMock(), getCurrentTime()).WillByDefault(Return(TimeValue(50, 0)));

    FlightTrajectory trajectory;
    trajectory.setPosition(Position(10, 20, 30));

    auto description = trajectory.getDescription();
    EXPECT_EQ(0, description.controlPoints.size());
}



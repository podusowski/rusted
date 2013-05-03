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
};

TEST_F(FlightTrajectoryTest, FullMovement)
{
    EXPECT_CALL(getSpline3Mock(), getLength()).Times(AtLeast(1)).WillRepeatedly(Return(100));
    EXPECT_CALL(getSpline3Mock(), empty()).Times(AtLeast(1)).WillRepeatedly(Return(false));
    EXPECT_CALL(getSpline3Mock(), reset()).Times(AtLeast(1));
    EXPECT_CALL(getSpline3Mock(), addControlPoint(_)).Times(AtLeast(1));
    EXPECT_CALL(getSpline3Mock(), derivative(_)).Times(AtLeast(1)).WillRepeatedly(Return(Position(0, 0, 1)));

    FlightTrajectory trajectory;

    trajectory.setPosition(Position(0, 0, 0));
    trajectory.setSpeed(10);
    trajectory.setAcceleration(10);

    EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(0, 0)));
    trajectory.fly(Position(0, 0, 100));
    Mock::VerifyAndClear(&getTimeMock());

    {
        InSequence s;

        // distance = 100
        // acceleration = a = 10
        // speed = 10


        // acceleration phase
        // S = at2/2
        // x = S/distance (100)
        // V = at
        {
            // t = 0.1
            // S = 10 * 0.1^2 / 2 = 0.05
            // V = 10 * 0.1 = 1
            // x = 0.05 / 100
            EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(0, 100)));
            EXPECT_CALL(getSpline3Mock(), operatorCall(0.0005)).Times(1);
            trajectory.getPosition();
            //EXPECT_EQ(1, trajectory.getCurrentSpeed());

            // t = 0.5
            // S = 10 * 0.5^2 / 2 = 1.25
            // x = 1.25 / 100
            EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(0, 500)));
            EXPECT_CALL(getSpline3Mock(), operatorCall(0.0125)).Times(1);
            trajectory.getPosition();
        }

        // steady phase
        // S = a * t1^2 / 2 + speed * (t - t1)
        // t1 = speed / acceleration = 10 / 10 = 1
        {
            // t = 1.1
            // S = 10 * 1 / 2 + 10 * (1.1 - 1) = 5 + 10 * 0.1 = 6
            // x = 6 / 100 = 0.06
            EXPECT_CALL(getTimeMock(), getCurrentTime()).Times(1).WillOnce(Return(TimeValue(1, 100)));
            EXPECT_CALL(getSpline3Mock(), operatorCall(0.06)).Times(1);
            trajectory.getPosition();
        }

        // deceleration phase
        // first triangle: T1 = a * t1^2 / 2
        // rectangle: R = speed * (t2 - t1)
        // second triangle part: a * (Tmax - t)^2 / 2
        // TODO
    }
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



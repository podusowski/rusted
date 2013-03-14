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
    ShipTest()
    {
        using namespace Cake::DependencyInjection;

        Cake::DependencyInjection::clear();

        m_time.reset(new RustedTimeStub);
        forInterface<Common::Game::IRustedTime>().use(m_time);

        m_trajectory.reset(new Common::Game::Object::FlightTrajectoryMock);
        forInterface<Common::Game::Object::IFlightTrajectory>().use(m_trajectory);
    }

    ~ShipTest()
    {
        Cake::DependencyInjection::clear();
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

TEST_F(ShipTest, SetCourse)
{
    Common::Game::Object::Ship ship;
    EXPECT_CALL(getTrajectoryMock(), fly(_)).Times(1);
    ship.setCourse(Common::Game::Position(1, 1, 1));
}

TEST_F(ShipTest, GetPosition)
{
    Common::Game::Object::Ship ship;
    EXPECT_CALL(getTrajectoryMock(), getPosition()).Times(1).WillRepeatedly(Return(Common::Game::Position(0, 0, 0)));
    ship.getPosition();
}

TEST_F(ShipTest, SetPosition)
{
    Common::Game::Object::Ship ship;
    EXPECT_CALL(getTrajectoryMock(), setPosition(_)).Times(1);
    ship.setPosition(Common::Game::Position(1, 2, 3));
}

TEST_F(ShipTest, SetSpeed)
{
    Common::Game::Object::Ship ship;
    EXPECT_CALL(getTrajectoryMock(), setSpeed(5)).Times(1);
    ship.setSpeed(5);
}

/* When ship is destroyed somewhere, it should stop moving and stay in place where it
 * was when it happened. 
 */
TEST_F(ShipTest, ShipDestroyedDuringFlight)
{
    Common::Game::Object::Ship ship;
    EXPECT_CALL(getTrajectoryMock(), stop()).Times(1);
    EXPECT_CALL(getTrajectoryMock(), fly(_)).Times(0);
    ship.setIntegrity(0);

    // shouldn't fly anymore
    ship.setCourse(Common::Game::Position(200, 0, 0));
}

/* Now, damaging ship doesn't affect anything 
 */
TEST_F(ShipTest, ShipDamagedDuringFlight)
{
    Common::Game::Object::Ship ship;

    EXPECT_CALL(getTrajectoryMock(), fly(_)).Times(1);
    EXPECT_CALL(getTrajectoryMock(), stop()).Times(0);

    ship.setCourse(Common::Game::Position(100, 0, 0));
    ship.setIntegrity(10);
}

/* Yeah, blame me for not doing OOP but we need internal course representation to send it over
 * the network
 */
TEST_F(ShipTest, GetTrajectoryDescription)
{
    Common::Game::Object::Ship ship;

    Common::Game::Object::IFlightTrajectory::Description description;
    description.controlPoints.push_back(Common::Game::Position(1, 2, 3));

    EXPECT_CALL(getTrajectoryMock(), getDescription()).Times(1).WillRepeatedly(Return(description));
 
    auto course = ship.getTrajectoryDescription();

    EXPECT_EQ(1u, course.controlPoints.size());
}

TEST_F(ShipTest, ApplyTrajectoryDescription)
{
    Common::Game::Object::Ship ship;

    Common::Game::Object::IFlightTrajectory::Description description;
    description.controlPoints.push_back(Common::Game::Position(1, 2, 3));

    EXPECT_CALL(getTrajectoryMock(), applyDescription(_)).Times(1);

    ship.applyTrajectoryDescription(description);
}

TEST_F(ShipTest, DefaultIntegrity)
{
    Common::Game::Object::Ship ship;
    EXPECT_EQ(100, ship.getIntegrity());
}

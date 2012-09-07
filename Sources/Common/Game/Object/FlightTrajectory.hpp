#pragma once

#include "Cake/DependencyInjection/Inject.hpp"
#include "IFlightTrajectory.hpp"

namespace Common
{
namespace Game
{
namespace Object
{

class FlightTrajectory : public IFlightTrajectory
{
public:
    FlightTrajectory();
    ~FlightTrajectory();

    void fly(Position destination);
    void stop();

    void setPosition(Position);
    Position getPosition();
    Common::Math::Quaternion getOrientation();

    void setSpeed(unsigned);
    unsigned getSpeed();

    Description getDescription();
    void applyDescription(Description);

private:
    Position calculatePosition(TimeValue time);

    Cake::DependencyInjection::Inject<IRustedTime> m_time;
    Description m_description;
    unsigned m_speed;
};

}
}
}


#pragma once

#include "Cake/DependencyInjection/Inject.hpp"

#include "Common/Game/Position.hpp"
#include "Common/Game/TimeValue.hpp"
#include "Common/Game/IRustedTime.hpp"

namespace Common
{
namespace Game
{
namespace Object
{

class FlightTrajectory
{
public:
    struct Description
    {
        Position start;
        Position destination;
        TimeValue startTime;
    };

    FlightTrajectory();

    void fly(Position destination);
    void stop();

    void setPosition(Position);
    Position getPosition();

    void setSpeed(unsigned);

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


#pragma once

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

    void fly(Position destination);
    Position getPosition();
};

}
}
}


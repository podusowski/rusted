#pragma once

#include "Common/Game/Position.hpp"
#include "Common/Game/TimeValue.hpp"
#include "Common/Game/IRustedTime.hpp"

namespace Common
{
namespace Game
{
namespace Object
{

class IFlightTrajectory
{
public:
    struct Description
    {
        Position start;
        Position destination;
        TimeValue startTime;
    };

    virtual void fly(Position destination) = 0;
    virtual void stop() = 0;

    virtual void setPosition(Position) = 0;
    virtual Position getPosition() = 0;

    // angle around Y-axis in radians
    virtual float getYaw() = 0;

    // angle around X-axis in radians
    virtual float getPitch() = 0;

    // angle around Z-axis in radians
    virtual float getRoll() = 0;

    virtual Position getTangent100() = 0;

    virtual void setSpeed(unsigned) = 0;
    virtual unsigned getSpeed() = 0;

    virtual Description getDescription() = 0;
    virtual void applyDescription(Description) = 0;

    virtual ~IFlightTrajectory() {}
};

}
}
}

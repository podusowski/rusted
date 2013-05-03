#pragma once

#include <vector>

#include "Common/Math/Quaternion.hpp"
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
        std::vector<Position> controlPoints;
        TimeValue startTime;
    };

    virtual void fly(Position destination) = 0;
    virtual void stop() = 0;

    virtual void setPosition(Position) = 0;
    virtual Position getPosition() = 0;

    virtual Position getCourseMarkerPosition() = 0;
    virtual Common::Math::Quaternion getCourseMarkerOrientation() = 0;

    virtual Common::Math::Quaternion getOrientation() = 0;

    virtual void setSpeed(unsigned) = 0;
    virtual unsigned getSpeed() = 0;

    virtual unsigned getCurrentSpeed() = 0;

    virtual bool isMoving() = 0;

    virtual Description getDescription() = 0;
    virtual void applyDescription(Description) = 0;

    virtual ~IFlightTrajectory() {}
};

}
}
}


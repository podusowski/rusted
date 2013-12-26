#pragma once

#include "Cake/DependencyInjection/Inject.hpp"
#include "IFlightTrajectory.hpp"
#include "Common/Math/ISpline3.hpp"

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
    Position getDestination();
    Common::Math::Quaternion getOrientation();
    void setOrientation(const Common::Math::Quaternion &);

    void setMaxSpeed(unsigned);
    unsigned getMaxSpeed();

    unsigned getCurrentSpeed();

    void setAcceleration(unsigned);

    bool isMoving();

    Description getDescription();
    void applyDescription(Description);

private:
    void recalculate();
    void configureBezier();
    Description compensateLag(const Description & description);
    Position calculateOrientationControlPoint(const Position &) const;

    Cake::DependencyInjection::Inject<IRustedTime> m_time;
    Description m_description;
    unsigned m_maxSpeed;
    unsigned m_acceleration;
    Cake::DependencyInjection::Inject<Common::Math::ISpline3> m_spline;

    Position m_cachedPosition;
    Common::Math::Quaternion m_cachedOrientation;
    unsigned m_cachedSpeed;
};

}
}
}


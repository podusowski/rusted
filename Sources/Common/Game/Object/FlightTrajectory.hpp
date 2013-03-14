#pragma once

#include "Cake/DependencyInjection/Inject.hpp"
#include "IFlightTrajectory.hpp"
#include "Common/Math/Bezier3.hpp"

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
    Position getCourseMarkerPosition();
    Common::Math::Quaternion getCourseMarkerOrientation();
    Common::Math::Quaternion getOrientation();

    void setSpeed(unsigned);
    unsigned getSpeed();

    Description getDescription();
    void applyDescription(Description);

private:
    Position calculatePosition(float progress);
    Common::Math::Quaternion calculateOrientation(float progress);
    float calculateProgress(TimeValue time);
    void configureBezier();
    void revalidateProgress(float progress);
    void calculateCachedPositionAndOrientation(float progress);

    Cake::DependencyInjection::Inject<IRustedTime> m_time;
    Description m_description;
    unsigned m_speed;
    Common::Math::Bezier3 m_bezier;

    Position m_cachedPosition;
    Common::Math::Quaternion m_cachedOrientation;
};

}
}
}


#pragma once

#include <gmock/gmock.h>

#include "Common/Game/Object/IFlightTrajectory.hpp"

namespace Common
{
namespace Game
{
namespace Object
{

class FlightTrajectoryMock : public IFlightTrajectory
{
public:
    MOCK_METHOD1(fly, void(Position));
    MOCK_METHOD0(stop, void());
    MOCK_METHOD1(setPosition, void(Position));
    MOCK_METHOD0(getPosition, Position());
    MOCK_METHOD0(getCourseMarkerPosition, Position());
    MOCK_METHOD0(getCourseMarkerOrientation, Common::Math::Quaternion());
    MOCK_METHOD0(getDestination, Position());
    MOCK_METHOD0(getOrientation, Common::Math::Quaternion());
    MOCK_METHOD1(setOrientation, void(const Common::Math::Quaternion &));
    MOCK_METHOD1(setMaxSpeed, void(unsigned));
    MOCK_METHOD0(getMaxSpeed, unsigned());
    MOCK_METHOD0(getCurrentSpeed, unsigned());
    MOCK_METHOD0(isMoving, bool());
    MOCK_METHOD0(getDescription, Description());
    MOCK_METHOD1(applyDescription, void(Description));
};

}
}
}


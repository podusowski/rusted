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
    MOCK_METHOD0(getYaw, float());
    MOCK_METHOD0(getPitch, float());
    MOCK_METHOD0(getRoll, float());
    MOCK_METHOD0(getTangent100, Position());
    MOCK_METHOD0(getOrientation, Common::Math::Quaternion());
    MOCK_METHOD1(setSpeed, void(unsigned));
    MOCK_METHOD0(getSpeed, unsigned());
    MOCK_METHOD0(getDescription, Description());
    MOCK_METHOD1(applyDescription, void(Description));
};

}
}
}


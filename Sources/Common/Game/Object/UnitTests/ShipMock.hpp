#pragma once

#include <gmock/gmock.h>

#include "Common/Game/Object/Ship.hpp"

namespace Common
{
namespace Game
{
namespace Object
{

class ShipMock : public Ship
{
public:
    MOCK_METHOD0(getPosition, Position());
    MOCK_METHOD1(setCourse, void(Position));
    MOCK_METHOD1(setSpeed, void(unsigned));
    MOCK_METHOD0(getIntegrity, unsigned());
    MOCK_METHOD1(setIntegrity, void(unsigned));
    MOCK_METHOD1(setModel, void(std::string));
    MOCK_METHOD0(getTrajectoryDescription, IFlightTrajectory::Description());
};

}
}
}

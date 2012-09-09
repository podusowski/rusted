#pragma once

#include <gmock/gmock.h>

#include "Game/IShipClassContainer.hpp"

namespace Server
{
namespace Game
{

class ShipClassContainerMock : public IShipClassContainer
{
public:
    MOCK_METHOD1(getById, IShipClass & (unsigned));
};

}
}

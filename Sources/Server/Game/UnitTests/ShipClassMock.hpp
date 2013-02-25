#pragma once

#include <gmock/gmock.h>

#include "Game/IShipClass.hpp"

namespace Server
{
namespace Game
{

class ShipClassMock : public IShipClass
{
public:
    MOCK_METHOD1(applyTo, void (Common::Game::Object::Ship &));
    MOCK_METHOD0(getAvailableActions, std::vector<AvailableAction>());
    MOCK_METHOD0(getId, unsigned());
    MOCK_METHOD0(getName, std::string());
    MOCK_METHOD0(getRequiredCarbon, unsigned());
    MOCK_METHOD0(getRequiredHelium, unsigned());
};

}
}

